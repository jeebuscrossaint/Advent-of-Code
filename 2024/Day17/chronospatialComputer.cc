#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <functional>

std::vector<uint64_t> getNumbersInLine(std::string line){
    std::regex numberRegex(R"(\d+)");
    std::vector<uint64_t> numbers;
    for (std::sregex_token_iterator it(line.begin(), line.end(), numberRegex), end; it != end; ++it)
        numbers.push_back(std::stoi(it->str()));
    return numbers;
}

std::pair<std::vector<uint64_t>, std::vector<uint64_t>> parseFile(std::string fileName)
{
    std::vector<uint64_t> registers, program;
    std::ifstream file(fileName);
    std::string line;
    std::vector<std::string> input;
    while(std::getline(file, line)) {
        if (line == "")
            break;
        registers.push_back(getNumbersInLine(line)[0]);
    }
    std::getline(file, line);
    program = getNumbersInLine(line);
    file.close();
    return {registers, program};
}

class Processor {
    private:
        std::vector<uint64_t> registers, program;
        uint64_t curInstruction;
        std::vector<std::function<void(uint64_t)>> instructionSet = {
            [this](auto operand) { registers[0] = registers[0] / std::pow(2, asCombo(operand)); }, // 0, adv, A = A // 2^(combo)
            [this](auto operand) { registers[1] ^= operand; }, // 1, bxk, B = B XOR literal
            [this](auto operand) { registers[1] = asCombo(operand) % 8; }, // 2, bst, B = combo % 8
            [this](auto operand) { if (registers[0] != 0) { curInstruction = 2 * operand; } }, // 3, jnz, if A != 0, jump to literal
            [this](auto operand) { registers[1] ^= registers[2]; }, // 4, bxc, B = B XOR C
            [this](auto operand) { output.push_back(asCombo(operand) % 8); }, // 5, out, print combo % 8
            [this](auto operand) { registers[1] = registers[0] / std::pow(2, asCombo(operand)); }, // 6, bdv, B = A // 2^(combo)
            [this](auto operand) { registers[2] = registers[0] / std::pow(2, asCombo(operand)); } // 7, cdv, C = A // 2^(combo)
        };
        uint64_t asCombo(uint64_t operand) {
            return operand < 4 ? operand : registers[operand - 4];
        }
        void step() {
            uint64_t opcode = program[curInstruction];
            uint64_t operand = program[curInstruction + 1];
            curInstruction += 2;
            instructionSet[opcode](operand);
        }
        void runProgram() {
            for (curInstruction = 0; curInstruction < program.size()-1;)
                step();
        }
    public:
        std::vector<uint64_t> output;
        Processor(std::vector<uint64_t> registers, std::vector<uint64_t> program): registers(registers), program(program) {}
        std::string getOutput() {
            runProgram();
            std::string s;
            for (int i=0; i<output.size(); i++) {
                if (i > 0)
                    s += ",";
                s += std::to_string(output[i]);
            }
            return s;
        }
        bool sameOutput(int instructionIdx) {
            runProgram();
            for (int i=0; i<output.size() && i<program.size(); i++)
                if (output[i] != program[instructionIdx + i])
                    return false;
            return true;
        }
};

uint64_t findRegistersProducingOutput(std::vector<uint64_t> program, uint64_t curInstruction, uint64_t currentA)
{
    for (uint64_t i = 0; i < 8; i++) {
        uint64_t nextA = currentA * 8 + i;
        if (Processor({nextA, 0, 0}, program).sameOutput(curInstruction)) {
            if (curInstruction == 0)
                return nextA;
            uint64_t result = findRegistersProducingOutput(program, curInstruction - 1, nextA);
            if (result != UINT64_MAX)
                return result;
        }
    }
    return UINT64_MAX;
}

void resolve() {
    auto [registers, program] = parseFile("input.txt");
    std::cout << "Part 1: " << Processor(registers, program).getOutput() << std::endl;
    std::cout << "Part 2: " << findRegistersProducingOutput(program, program.size()-1, 0) << std::endl;
}

int main()
{
    resolve();
    return 0;
}
