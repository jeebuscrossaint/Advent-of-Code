#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <cstdint>

struct PlantMap {
    uint32_t to;
    uint32_t from;
    uint32_t length;
};

struct PlantRange {
    uint32_t start;
    uint32_t end;
};

struct PlantData {
    std::vector<uint32_t> seeds_primary;
    std::vector<PlantRange> seeds_secondary;

    std::vector<PlantMap> seed_to_soil;
    std::vector<PlantMap> soil_to_fertilizer;
    std::vector<PlantMap> fertilizer_to_water;
    std::vector<PlantMap> water_to_light;
    std::vector<PlantMap> light_to_temperature;
    std::vector<PlantMap> temperature_to_humidity;
    std::vector<PlantMap> humidity_to_location;
};

PlantData generatePlantData(const std::string& input) {
    std::istringstream stream(input);
    std::string line;

    PlantData plant;

    std::getline(stream, line);
    std::istringstream seedsLine(line.substr(7));
    uint32_t seed;
    while (seedsLine >> seed) {
        plant.seeds_primary.push_back(seed);
    }

    std::vector<std::pair<uint32_t, uint32_t>> seedRanges;
    while (std::getline(stream, line)) {
        if (line.empty())
            break;

        std::istringstream rangeLine(line);
        uint32_t to, from, length;
        rangeLine >> to >> from >> length;
        plant.seeds_secondary.push_back({from, from + length - 1});
    }

    std::vector<std::vector<PlantMap>*> mapPointers = {
        &plant.seed_to_soil, &plant.soil_to_fertilizer, &plant.fertilizer_to_water,
        &plant.water_to_light, &plant.light_to_temperature, &plant.temperature_to_humidity,
        &plant.humidity_to_location
    };
    for (auto& mappings : mapPointers) {
        std::getline(stream, line);
        while (std::getline(stream, line)) {
            if (line.empty())
                break;

            std::istringstream mappingLine(line);
            PlantMap map;
            mappingLine >> map.to >> map.from >> map.length;
            mappings->push_back(map);
        }
    }

    return plant;
}

uint32_t processPlant(const PlantData& plant) {
    std::vector<uint32_t> state = plant.seeds_primary;

    auto applyMapping = [](const std::vector<uint32_t>& oldState, const std::vector<PlantMap>& mapping) -> std::vector<uint32_t> {
        std::vector<uint32_t> result;
        for (uint32_t previous : oldState) {
            for (const PlantMap& m : mapping) {
                uint32_t p = previous;
                uint32_t s = m.from;
                uint32_t l = m.length;
                uint32_t d = m.to;

                if (p >= s && p < s + l) {
                    result.push_back((p - s) + d);
                    goto nextPrevious;
                }
            }
            result.push_back(previous);

        nextPrevious:
            continue;
        }
        return result;
    };

    state = applyMapping(state, plant.seed_to_soil);
    state = applyMapping(state, plant.soil_to_fertilizer);
    state = applyMapping(state, plant.fertilizer_to_water);
    state = applyMapping(state, plant.water_to_light);
    state = applyMapping(state, plant.light_to_temperature);
    state = applyMapping(state, plant.temperature_to_humidity);
    state = applyMapping(state, plant.humidity_to_location);

    return *std::min_element(state.begin(), state.end());
}

int main() {
    std::ifstream inputFile("input.txt");
    std::stringstream buffer;
    std::string input;

    if (inputFile.is_open()) {
        buffer << inputFile.rdbuf();
        inputFile.close();
        input = buffer.str();
    } else {
        std::cerr << "Error." << std::endl;
    }

    PlantData plantData = generatePlantData(input);

    std::cout << processPlant(plantData) << std::endl;

    return 0;
}
