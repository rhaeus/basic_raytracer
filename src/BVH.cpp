#include "BVH.h"

BVH::BVH(std::vector<std::shared_ptr<Renderable>> objects)
{
    // calculate total scene bounds
    AABB bounds = objects.at(0)->getBounds();
    for (auto o : objects)
    {
        bounds = bounds + o->getBounds();
    }

    root = std::make_shared<Node>();
    root->aabb = bounds;
    insertObjects(objects);
}

Intersection BVH::intersect(const Ray& ray)
{
    return root->intersect(ray);
}

void BVH::insertObjects(std::vector<std::shared_ptr<Renderable>> objects)
{
    for (auto o : objects) {
        insert(root, o, 0);
    }

    computeBounds(root);
}

void BVH::computeBounds(std::shared_ptr<Node> node) 
{
    if (node->isLeaf) {
        // compute bounds of contents
        AABB bounds;
        if (node->content.size() > 0) {
            bounds = node->content.at(0)->getBounds();
        }
        for (auto c : node->content) {
            bounds = bounds + c->getBounds();
        }
        node->aabb = bounds;
    } else {
        // decend in tree
        for (int i = 0; i < node->children.size(); ++i) {
            if (node->children.at(i)) {
                computeBounds(node->children.at(i));
                node->aabb = node->aabb + node->children.at(i)->aabb;
            }
        }
    }
}

void BVH::insert(std::shared_ptr<Node> node, std::shared_ptr<Renderable> object, int depth) 
{
    if (node->isLeaf) { // a leaf
        // if nothing in the leaf or we reached max depth, insert here
        if (node->content.size() == 0 || depth == MAX_DEPTH){
            node->content.push_back(object);
        } else { // otherwise make new node and insert existing and new content there as children
            // make node a new Node (not leaf anymore)
            node->isLeaf = false;
            // move the content 
            while (node->content.size()){
                insert(node, node->content.back(), depth);
                node->content.pop_back();
            }
            insert(node, object, depth);
        }

    } else { // not a leaf: put object into correct child cell
        glm::vec3 objectCenter = object->getBounds().getCenter();
        glm::vec3 nodeCenter = node->aabb.getCenter();
        int childIndex = 0;

        if (objectCenter.x > nodeCenter.x) {
            childIndex += 4;
        } 
        if (objectCenter.y > nodeCenter.y) {
            childIndex += 2;
        }
        if (objectCenter.z > nodeCenter.z) {
            childIndex += 1;
        }

        if (node->children.at(childIndex) == nullptr) {
            node->children.at(childIndex) = std::make_shared<Node>();
            node->children.at(childIndex)->depth = depth;

            glm::vec3 childMin(0,0,0);
            glm::vec3 childMax(0,0,0);

            switch(childIndex) {
                case 0:
                    childMin = node->aabb.getMin();
                    childMax = nodeCenter;
                    break;
                case 1:
                    childMin = node->aabb.getMin();
                    childMin.z = nodeCenter.z;
                    childMax = nodeCenter;
                    childMax.z = node->aabb.getMax().z;
                    break;
                case 2:
                    childMin = node->aabb.getMin();
                    childMin.y = nodeCenter.y;
                    childMax = nodeCenter;
                    childMax.y = node->aabb.getMax().y;
                case 3:
                    childMin = nodeCenter;
                    childMin.x = node->aabb.getMin().x;
                    childMax = node->aabb.getMax();
                    childMax.y = nodeCenter.y;
                    break;
                case 4:
                    childMin = node->aabb.getMin();
                    childMin.x = nodeCenter.x;
                    childMax = nodeCenter;
                    childMax.x = node->aabb.getMax().x;
                    break;
                case 5:
                    childMin = nodeCenter;
                    childMin.y = node->aabb.getMin().y;
                    childMax = nodeCenter;
                    childMax.x = node->aabb.getMax().x;
                    childMax.z = node->aabb.getMax().z;
                    break;
                case 6:
                    childMin = nodeCenter;
                    childMin.z = node->aabb.getMin().z;
                    childMax = nodeCenter;
                    childMax.x = node->aabb.getMax().x;
                    childMax.y = node->aabb.getMax().y;
                    break;
                case 7:
                    childMin = nodeCenter;
                    childMax = node->aabb.getMax();
                    break;
            }

            node->children.at(childIndex)->aabb = AABB(childMin, childMax);
        }

        insert(node->children.at(childIndex), object, depth + 1);

    }
    
}