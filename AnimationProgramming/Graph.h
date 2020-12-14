#pragma once

#include <vector>

template<typename T>
class Graph
{
public:
    std::vector<Graph> children;
    T data;

    Graph() = default;

    template <typename... TArgs>
     Graph(TArgs... args)
         : children{}, data {args...}
    {}

    //Graph(const T& _data)
    //{
    //    data = _data;
    //}

    ~Graph() = default;

    void setData(const T& _data)
    {
        data = _data;
    }

    template <typename... TArgs>
    void addChild(TArgs... args)
    {
        Graph graph(args...);
        children.push_back(graph);
    }
};
