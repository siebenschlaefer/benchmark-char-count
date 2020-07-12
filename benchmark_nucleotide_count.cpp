#include <climits>
#include <iostream>
#include <random>

#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>
#include <spy.hpp>

std::string random_string(std::size_t size, const std::string_view alphabet)
{
    ankerl::nanobench::Rng rng(std::random_device{}());
    std::uniform_int_distribution<std::size_t> dist(0, alphabet.size() - 1);
    std::string str(size, ' ');
    std::generate(begin(str), end(str), [&](){ return alphabet[dist(rng)]; });
    return str;
}

int main()
{
    std::cout << "OS:           " << spy::operating_system     << '\n';
    std::cout << "architecture: " << spy::architecture         << '\n';
    std::cout << "SIMD:         " << spy::simd_instruction_set << '\n';
    std::cout << "Compiler:     " << spy::compiler             << '\n';
    std::cout << "libc:         " << spy::stdlib               << '\n';
    std::cout << "stdlibc:      " << spy::libc                 << '\n';

    const std::string_view alphabet = "ACGT";
    const auto str = random_string(10'000, alphabet);

    ankerl::nanobench::Bench bench;
    bench.minEpochIterations(5'000);

    bench.run("increment operator[]", [&] {
        if (str.find_first_not_of(alphabet) != std::string::npos)
            throw std::invalid_argument("invalid nucleotide");
        std::map<char, int> counter;
        for (auto c: alphabet)
            counter[c] = 0;
        for (auto c: str)
            ++counter[c];
        ankerl::nanobench::doNotOptimizeAway(counter);
    });
    bench.run("assign std::count()", [&] {
        std::map<char, int> counter;
        for (auto c: alphabet)
            counter[c] = static_cast<int>(std::count(str.begin(), str.end(), c));
        if (str.size() != std::accumulate(begin(counter), end(counter),
            std::size_t{}, [](auto acc, auto& pair) {
                return acc + static_cast<std::size_t>(pair.second);
            }))
            throw std::invalid_argument("invalid nucleotide");
        ankerl::nanobench::doNotOptimizeAway(counter);
    });
    bench.run("use array for counting", [&] {
        std::array<int, CHAR_MAX - CHAR_MIN + 1> arr{};
        for (auto c: str)
            ++arr[static_cast<std::size_t>(c - CHAR_MIN)];
        std::map<char, int> counter;
        for (auto c: alphabet)
            counter[c] = arr[static_cast<std::size_t>(c - CHAR_MIN)];
        if (str.size() != std::accumulate(begin(alphabet), end(alphabet),
            std::size_t{0}, [&arr](auto acc, auto c) {
                return acc + static_cast<std::size_t>(arr[static_cast<std::size_t>(c - CHAR_MIN)]);
            }))
            throw std::invalid_argument("invalid nucleotide");
        ankerl::nanobench::doNotOptimizeAway(counter);
    });
}

