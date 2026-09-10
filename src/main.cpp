#include <iostream>
#include <spdlog/spdlog.h>
#include <CLI/CLI.hpp>
#include "logger.hpp"
#include <thread>
#include <csignal>
#include <atomic>

static std::atomic<bool> g_stop{false};

static void on_sigterm(int) { g_stop.store(true, std::memory_order_relaxed); }

using namespace std::chrono_literals;

int main(int argc, char** argv)
{
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
    while(!g_stop.load(std::memory_order_relaxed)) {
        std::this_thread::sleep_for(2s);
        logger->info("Waiting for reader...");
    }
    logger->info("Stopping reader...");
    return 0;
}
