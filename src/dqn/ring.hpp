#pragma once

#include <random>
#include <stdexcept>
#include <vector>
#include "../pong/EpisodeParameter.hpp"
#include "ring.hpp"

class Ring
{
public:
    Ring(size_t capacity)
        : m_capacity(capacity)
    {
        m_memory.reserve(capacity);
    }

    EpisodeParameter& operator[](size_t index)
    {
        return m_memory[index % m_capacity];
    }

    const EpisodeParameter& operator[](size_t index) const
    {
        return m_memory[index % m_capacity];
    }

    const std::tuple< EpisodeParameter, Eigen::Matrix<float, 6, 1>, bool> Ring::sample()
    {
        size_t index = dis(gen);
        EpisodeParameter state = m_memory[index];

        size_t next_index = (index + 1) % m_capacity;
        const Eigen::Matrix<float, 6, 1>& next_state = m_memory[next_index].pongVariables;

        bool ended = m_memory[next_index].gameEnd;
        
        return {state, next_state, ended};
    };


private:
    std::vector<EpisodeParameter> m_memory;
    size_t m_capacity; 
    size_t size;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;
};