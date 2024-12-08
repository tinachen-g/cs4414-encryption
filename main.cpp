#include <vector>
#include <cassert>

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <future>
#include <mutex>
#include <algorithm>

#include "operation.hpp"
#include "bignum.hpp"

// #include <gperftools/profiler.h>

// Bignum RSA_N = Bignum("9616540267013058477253762977293425063379243458473593816900454019721117570003248808113992652836857529658675570356835067184715201230519907361653795328462699");
// Bignum RSA_E = Bignum("65537");
// Bignum RSA_D = Bignum("4802033916387221748426181350914821072434641827090144975386182740274856853318276518446521844642275539818092186650425384826827514552122318308590929813048801");

std::string RSA_N = "9616540267013058477253762977293425063379243458473593816900454019721117570003248808113992652836857529658675570356835067184715201230519907361653795328462699";
std::string RSA_E = "65537";
std::string RSA_D = "4802033916387221748426181350914821072434641827090144975386182740274856853318276518446521844642275539818092186650425384826827514552122318308590929813048801";

// std::vector<std::string> preprocessLine(const std::string& input, int lineNumber) {
// std::string paddedLine = " " + std::to_string(lineNumber) + input;
// while (paddedLine.size() < 99) {
//     paddedLine += " ";
// }
// paddedLine += std::to_string(lineNumber);
// if (paddedLine.size() > 102) {
//     paddedLine = paddedLine.substr(0, 102);
// }
// return {paddedLine.substr(0, 51), paddedLine.substr(51, 51)};
// }

std::vector<std::string> preprocessLine(std::string line, int line_number) {
    // total line space is 102 characters -> 6 will be for line numbers, so 96 remaining
    // truncates the line if exceeds 96 chacracyers
    line = line.substr(0, 96);

    // determining the line-counting 3 ascii char for the 3-digit line number
    std::string spaces = (line_number > 99) ? "" : ((line_number > 9) ? " " : "  ");

    std::string line_count = spaces + std::to_string(line_number);
    assert(line_count.length() == 3 && "the line_count prefix/suffix should be length of 3!");
    std::string padding(102 - 6 - line.size(), ' ');

    std::string padded_line = line_count + line + padding + line_count;
    std::string first_half = padded_line.substr(0, 51);
    std::string second_half = padded_line.substr(51, 102);

    assert(first_half.length() == 51 && "first half should be 51 characters");
    assert(second_half.length() == 51 && "second half should be 51 characters");

    // std::cout << "first 51 is \"" << first_half << "\"" << std::endl;
    // std::cout << "last 51 is \"" << second_half << "\"" << std::endl;
    return {first_half, second_half};
}

std::string lineIntoInts(const std::string &input) {
    std::string encoded = "";
    for (char c : input) {
        encoded += std::to_string(1000 + static_cast<int>(c)).substr(1);
    }
    return encoded;
}

// void encrypt(std::istream &input)
// {
//     std::string line;
//     int line_number = 1;
//     // reads one line at a tiem
//     while (std::getline(input, line))
//     {

//         std::vector<std::string> halves = preprocessLine(line, line_number);

//         // encryption formula is m^e % n
//         // TODO: should paralleize everything
//         std::string first_half = lineIntoInts(halves[0]);
//         Bignum first = encryption(first_half, RSA_E, RSA_N);
//         // std::cout << "first Bignum " << first.to_string() << std::endl;
//         std::cout << first.to_string() << std::endl;

//         std::string second_half = lineIntoInts(halves[1]);
//         Bignum second = encryption(second_half, RSA_E, RSA_N);
//         // std::cout << "second Bignum " << second.to_string() << std::endl;
//         std::cout << second.to_string() << std::endl;

//         line_number++;
//     }
//     return;
// }

void encrypt(std::istream &input) {
    std::vector<std::string> lines;
    std::string line;

    // read all lines from input and put into vectr
    while (std::getline(input, line)) {
        lines.push_back(line);
    }

    int total_lines = lines.size();
    if (total_lines == 0) {
        return;
    }

    const int k = std::thread::hardware_concurrency();
    int num_threads = k > 0 ? k : 5;

    std::vector<std::string> encrypted_results(total_lines);
    std::vector<std::thread> threads;
    std::mutex output_mutex;

    // divide work among threads
    auto encryptChunk = [&](int start, int end) {
        for (int i = start; i < end; ++i) {
            const std::string &line = lines[i];
            std::vector<std::string> halves = preprocessLine(line, i + 1);

            std::string first_half = lineIntoInts(halves[0]);
            Bignum first = encryption(first_half, RSA_E, RSA_N);

            std::string second_half = lineIntoInts(halves[1]);
            Bignum second = encryption(second_half, RSA_E, RSA_N);

            {
                std::lock_guard<std::mutex> lock(output_mutex);
                encrypted_results[i] = first.to_string() + "\n" + second.to_string();
            }
        }
    };

    int chunk_size = (total_lines + num_threads - 1) / num_threads;
    for (int t = 0; t < num_threads; ++t) {
        int start = t * chunk_size;
        int end = std::min(start + chunk_size, total_lines);
        if (start < end) {
            threads.emplace_back(encryptChunk, start, end);
        }
    }

    // wait for all threads to finish
    for (std::thread &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    for (const auto &encrypted_line : encrypted_results) {
        std::cout << encrypted_line << std::endl;
    }
}

// void decrypt(std::istream &input)
// {
//     std::string line1, line2;
//     while (std::getline(input, line1))
//     {
//         // read the next line
//         if (!std::getline(input, line2))
//         {
//             std::cout << "Warning: Odd number of lines. Possible error. Last line will be ignored." << std::endl;
//             break;
//         }

//         // decrypt both parts
//         std::string first_line = decryption(line1, RSA_D, RSA_N);
//         std::cout << "first line is \"" << first_line << "\"" << std::endl;
//         std::string second_line = decryption(line2, RSA_D, RSA_N);
//         std::cout << "second line is " << second_line << std::endl;

//         std::string combined_line = first_line + second_line;

//         std::cout << combined_line << std::endl;
//     }
//     return;
// }

void remove_trailing_spaces(std::string &str) {
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch)
                           { return !std::isspace(ch); })
                  .base(),
              str.end());
}

// void decrypt(std::istream &input)
// {
//     std::string line1, line2;
//     while (std::getline(input, line1))
//     {
//         // read the next line
//         if (!std::getline(input, line2))
//         {
//             std::cout << "Warning: Odd number of lines. Possible error. Last line will be ignored." << std::endl;
//             break;
//         }

//         std::string first_line, second_line;
//         std::mutex output_mutex;

//         // decrypt a line and store the result
//         auto decrypt_line = [](const std::string &line, std::string &result, std::mutex &mutex)
//         {
//             result = decryption(line, RSA_D, RSA_N);
//             {
//                 std::lock_guard<std::mutex> lock(mutex);
//             }
//         };

//         std::thread first_thread(decrypt_line, line1, std::ref(first_line), std::ref(output_mutex));
//         std::thread second_thread(decrypt_line, line2, std::ref(second_line), std::ref(output_mutex));

//         // wait for both threads to finish
//         first_thread.join();
//         second_thread.join();
//         std::string combined_line = first_line + second_line;

//         if (combined_line.size() > 6)
//         {
//             combined_line = combined_line.substr(3, combined_line.size() - 6);
//         }
//         else
//         {
//             combined_line.clear();
//         }

//         remove_trailing_spaces(combined_line);

//         std::cout << combined_line << std::endl;
//     }
//     return;
// }

void decrypt(std::istream &input) {
    std::string line1, line2;
    std::vector<std::string> results;
    std::mutex output_mutex;

    auto decrypt_line = [](const std::string &line, std::string &result)
    {
        result = decryption(line, RSA_D, RSA_N);
    };

    while (std::getline(input, line1)) {
        if (!std::getline(input, line2)) {
            std::cout << "Warning: Odd number of lines. Possible error. Last line will be ignored." << std::endl;
            break;
        }

        std::string first_line, second_line;
        std::thread first_thread(decrypt_line, line1, std::ref(first_line));
        std::thread second_thread(decrypt_line, line2, std::ref(second_line));

        first_thread.join();
        second_thread.join();
        std::string combined_line = first_line + second_line;

        if (combined_line.size() > 6) {
            combined_line = combined_line.substr(3, combined_line.size() - 6);
        }
        else {
            combined_line.clear();
        }

        remove_trailing_spaces(combined_line);
        results.push_back(combined_line);
    }

    for (const auto &line : results) {
        std::lock_guard<std::mutex> lock(output_mutex);
        std::cout << line << std::endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./bignum <e/d> < text.txt"  << std::endl;
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "e") {
        encrypt(std::cin);
    } else if (mode == "d") {
        decrypt(std::cin);
    } else {
        std::cerr << "Invalid mode. Use 'e' for encryption or 'd' for decryption." << std::endl;
        return 1;
    }

    return 0;
}