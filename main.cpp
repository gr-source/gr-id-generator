#include <unordered_map>
#include <iostream>
#include <vector>
#include <chrono>
#include <bitset>

using ComponentHash = std::size_t;
using ComponentID = std::size_t;

class ComponentIDGenerator {
public:
    ComponentIDGenerator() : m_idCount(0) {
    }

    ComponentID GenerateIDByName(const std::string &name) noexcept {
        return GenerateIDByKey(name);
    }

    template <typename T>
    ComponentID GenerateIDByType() noexcept{
        return GenerateIDByKey(typeid(T).name());
    }

    void DeleteIDByName(const std::string &name) noexcept {
        return DeleteIDByKey(name);
    }

    template <typename T>
    void DeleteID() noexcept {
        return DeleteIDByKey(typeid(T).name());
    }

    void print() {
        std::cout << "===========================================" << std::endl;
        std::cout << "ComponentCount: " << m_idCount << std::endl;
        std::cout << "Total Available: " << m_available.size() << std::endl;
        std::cout << "Cache: " << m_hash.size() << std::endl;
    }

private:
    ComponentID GenerateIDByKey(const std::string &key) noexcept {
        ComponentID id = -1;

        auto it = m_hash.find(key);
        if (it != m_hash.end()) {
            return (*it).second;
        }

        auto available = !m_available.empty();
        
        id = available ? m_available.back() : GenerateID();
        if (available) {
            m_available.pop_back();
        }

        m_hash[key] = id;
        m_test[id] = true;

        return id;
    }

    void DeleteIDByKey(const std::string &key) noexcept {
        auto it = m_hash.find(key);
        if (it == m_hash.end()) {
            return;
        }

        auto id = it->second;

        m_available.push_back(id);

        m_test[id] = false;

        m_hash.erase(it);
    }

    ComponentID GenerateID() noexcept {
        return m_idCount++;
    }

    std::unordered_map<std::string, ComponentID> m_hash;

    std::vector<ComponentID> m_available;

    std::bitset<100000> m_test;

    ComponentID m_idCount;
};

int main() {
    ComponentIDGenerator generator;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 100000; ++i) {
        generator.GenerateIDByName(std::to_string(i));
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Tempo para gerar IDs: " << elapsed.count() << " segundos\n";

    generator.print();

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 100000; ++i) {
        // generator.DeleteID<int>();
        generator.DeleteIDByName(std::to_string(i));
    }

    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;

    std::cout << "Tempo para excluir IDs: " << elapsed.count() << " segundos\n";

    generator.print();

    return 0;
}

/*
int main() {
    ComponentIDGenerator generator;

    ComponentID id = -1;

    id = generator.GenerateIDByType<int>();
    generator.print();

    id = generator.GenerateIDByType<float>();
    generator.print();

    generator.DeleteID<float>();
    generator.print();

    // id = generator.GenerateIDByType<float>();
    // generator.print();
}




*/