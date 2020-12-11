#pragma once

#include <vector>

template<typename T>
class Graph
{
public:
    std::vector<Graph> children;
    T data;

    Graph() = default;
    Graph(const T& _data)
    {
        data = _data;
    }
    ~Graph() = default;

    void setData(const T& _data)
    {
        data = _data;
    }
    void addChild(const T& _data)
    {
        Graph graph(_data);
        children.push_back(graph);
    }
};
