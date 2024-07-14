#include "node.h"
#include <algorithm>
#include <memory>
using namespace Astrocore;

int Node::NODE_INCREMENTOR = 0;

Node::Node()
{
    SetNodeID();
    this->transform = std::unique_ptr<Transform2D>(new Transform2D());
    this->worldTransform = std::unique_ptr<Transform2D>(new Transform2D());
    children = std::vector<Node*>();
}

Node::Node(std::string name):Node()
{
    this->name = name;
}

Node::~Node()
{
    //if(isInTree)
    //{
    //    // TODO: De-register from tree (which will call OnTreeExit)
    //}

    if(parent != nullptr)
    {
        // Remove ourselves from the child
        parent->RemoveChild(this);
    }
   
    // Delete children
    for(Node* child : children)
    {
        delete child;
        child = nullptr;
    }
}

void Node::SetNodeID()
{
    int id = NODE_INCREMENTOR;
    NODE_INCREMENTOR++;
    nodeID = id;
}

int Node::GetNodeID()
{
    return nodeID;
}

void EnterTree(SceneTree* tree)
{
    //tree->RegisterToTree(TreeNode::weak_from_this());
}
void Node::AddChild(Node* newChild)
{
    std::vector<Node*>::iterator it = std::find(children.begin(), children.end(), newChild);

    // Already has the child: don't do anything
    if(it != children.end())
    {
        return;
    }
    children.push_back(newChild);
    newChild->SetParent(this);
}

void Node::RemoveChild(Node* childToRemove)
{
     std::vector<Node*>::iterator it = std::find(children.begin(), children.end(), childToRemove);

    if(it != children.end())
    {
        children.erase(it);
        return; 
    }
}

void Node::SetInheritsParentTransform(bool shouldInheritParentTransform)
{
    this->inheritParentTransform = shouldInheritParentTransform;
}

void Node::SetParent(Node* newParent)
{
    if(parent != nullptr)
    {
        parent->RemoveChild(this);
    }
    if(newParent != parent)
    {
        parent = newParent;
        parent->AddChild(this);
    }
}

Node* Node::GetChildAtIndex(int index)
{
    // Bounds check
    if(index < 0 || index > children.size())
    {
        return nullptr;
    }
    return children.at(index);
}

void Node::Update(float deltaTime)
{
    for(Node* child : children)
    {
        child->Update(deltaTime);
    }
}

Transform2D* Node::GetTransform()
{
    // TODO: Do this only when the transform changes
    for(Node* child : children)
    {
        child->SetIsWorldMatrixDirty(true);
    }

    return transform.get();
}

Transform2D Node::GetWorldTransform()
{
    if(parent == nullptr || !inheritParentTransform)
    {
        Transform2D* t = this->transform.get();
        return Transform2D(t);
    }
    

    // TODO: Fix the dirty flag here
    //if(isWorldMatrixDirty)
    //{
    worldTransform->SetMatrix(MatrixMultiply(transform->GetMatrix(), parent->GetWorldTransform().GetMatrix()));
    //isWorldMatrixDirty = false;
    //}


    return *worldTransform;
}
