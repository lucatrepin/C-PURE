#ifndef NODE_HPP
#define NODE_HPP

namespace Node
{
class Node2D {
public:
    Node2D() {
        Start();
    }
    virtual void Start() {}
    virtual void Process(float delta) = 0;
};
}

#endif