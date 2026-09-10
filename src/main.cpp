#include <iostream>
#include <spdlog/spdlog.h>
#include <CLI/CLI.hpp>
#include "logger.hpp"
#include <thread>
#include <csignal>
#include <atomic>
#include "SocketCtl.h"
#include "SpecialId.h"
static std::atomic<bool> g_stop{false};

static void on_sigterm(int) { g_stop.store(true, std::memory_order_relaxed); }

using namespace std::chrono_literals;

int main(int argc, char** argv) {
    struct sigaction sa{};
    sa.sa_handler = on_sigterm;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGTERM, &sa, nullptr);
    sigaction(SIGINT,  &sa, nullptr);

    const auto logger = Logger::get();

    CLI::App app{"Can reader"};
    std::string interface;
    app.add_option("-i,--interface", interface,
        "Can interface to capture on")->required();
    CLI11_PARSE(app, argc, argv);
    logger->info("CAN READER");
    logger->info("Interface: {}", interface);
    can_reader::SocketCtl ctl{};
    if (const auto err = ctl.open_socket(interface); err) {
        logger->error("Can't open socket: {}", err.message());
        return err.value();
    }
    while(!g_stop.load(std::memory_order_relaxed)) {
        can::Frame frame {};
        if (const auto err = ctl.read(frame); err) {
            logger->error("Read CAN frame error: {}", err.message());
        }
        else {
            std::string dataHex;
            for (auto i : frame) {
                dataHex += fmt::format("{:02X} ", i);
            }
            fmt::println("{:08X},{}", frame.id, dataHex);
        }
        if (frame.id == 0x1F1122EE) {
            try {
                auto special = can_reader::SpecialId_0x1F1122EE::deserialize(frame);
                fmt::println("{}", special);
            }
            catch (const std::length_error& e) {
                logger->error("Bad frame: {}", e.what());
            }catch (const std::exception& e) {
                logger->error("Bad frame unexpected: {}", e.what());
            } catch (...) {
                logger->error("Bad frame: non-std exception");
            }
        }

    }
    logger->info("Reader was stopped");
    return 0;
}
