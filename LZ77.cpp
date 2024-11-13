#include <iostream>
#include <vector>
#include <tuple>
#include <string>

using namespace std;

struct Token {
    int offset;
    int length;
    char nextChar;
};

std::vector<Token> encode(const std::string& input) {
    std::vector<Token> tokens;
    int windowSize = 4096; // Example window size
    int lookaheadSize = 15; // Example lookahead buffer size

    for (size_t i = 0; i < input.size(); ++i) {
        int maxMatchLength = 0;
        int bestOffset = 0;
        for (int j = std::max(0, static_cast<int>(i) - windowSize); j < i; ++j) {
            int matchLength = 0;
            while (matchLength < lookaheadSize && i + matchLength < input.size() &&
                   input[j + matchLength] == input[i + matchLength]) {
                ++matchLength;
            }
            if (matchLength > maxMatchLength) {
                maxMatchLength = matchLength;
                bestOffset = i - j;
            }
        }
        if (maxMatchLength > 0) {
            tokens.push_back({bestOffset, maxMatchLength, input[i + maxMatchLength]});
            i += maxMatchLength; // Skip over the matched part
        } else {
            tokens.push_back({0, 0, input[i]});
        }
    }
    return tokens;
}

std::string decode(const std::vector<Token>& tokens) {
    std::string output;
    for (const auto& token : tokens) {
        if (token.offset == 0 && token.length == 0) {
            output += token.nextChar;
        } else {
            int start = output.size() - token.offset;
            for (int i = 0; i < token.length; ++i) {
                output += output[start + i];
            }
            output += token.nextChar;
        }
    }
    return output;
}

int main() {
    cout << "write your text: ";
    std::string input;
    getline(cin, input);
    auto encoded = encode(input);
    auto decoded = decode(encoded);

    // Print the original string
    std::cout << "Original: " << input << std::endl;

    // Print the encoded tokens
    std::cout << "Encoded Tokens: " << std::endl;
    for (const auto& token : encoded) {
        std::cout << "(" << token.offset << ", " << token.length << ", '" << token.nextChar << "')" << std::endl;
    }

    // Print the decoded string
    std::cout << "\nDecoded: " << decoded << std::endl;

    return 0;
}
