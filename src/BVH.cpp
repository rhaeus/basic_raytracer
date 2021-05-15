#include "BVH.h"

BVH::BVH(std::vector<std::shared_ptr<Renderable>> objects)
{
    AABB aabb(glm::vec3(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()), 
        glm::vec3(std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min()));

    root = std::make_shared<Node>(std::make_shared<AABB>(aabb));

    // calculate total scene bounds
    if (objects.size() > 0)
    {
        root->aabb = objects.at(0)->getBounds();
    }

    for (auto o : objects)
    {
        *root->aabb  = *root->aabb  + *o->getBounds();
    }

    std::cout << "Scene bounds: " << std::endl 
        << "min " << root->aabb->getMin().x << ", " << root->aabb->getMin().y << ", " << root->aabb->getMin().z << std::endl
        << "max " << root->aabb->getMax().x << ", " << root->aabb->getMax().y << ", " << root->aabb->getMax().z << std::endl;

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
        if (node->content.size() > 0) {
            node->aabb = node->content.at(0)->getBounds();
        }
        for (auto c : node->content) {
            *node->aabb = *node->aabb + *c->getBounds();
        }
    } else {
        // decend in tree
        for (int i = 0; i < node->children.size(); ++i) {
            if (node->children.at(i)) {
                computeBounds(node->children.at(i));
                *node->aabb = *node->aabb + *node->children.at(i)->aabb;
            }
        }
    }
}

void computeChildBound( 
    const int &i, const glm::vec3 &boundCentroid, 
    const glm::vec3 &boundMin, const glm::vec3 &boundMax, 
    glm::vec3 &pMin, glm::vec3 &pMax)  
{ 
    pMin[0] = (i & 4) ? boundCentroid[0] : boundMin[0]; 
    pMax[0] = (i & 4) ? boundMax[0] : boundCentroid[0]; 
    pMin[1] = (i & 2) ? boundCentroid[1] : boundMin[1]; 
    pMax[1] = (i & 2) ? boundMax[1] : boundCentroid[1]; 
    pMin[2] = (i & 1) ? boundCentroid[2] : boundMin[2]; 
    pMax[2] = (i & 1) ? boundMax[2] : boundCentroid[2]; 
} 

void BVH::insert(std::shared_ptr<Node> node, std::shared_ptr<Renderable> object, int depth) 
{
    // std::cout << "depth: " << depth << std::endl;
    if (node->isLeaf) { // a leaf
        // if nothing in the leaf or we reached max depth, insert here
        if (node->content.size() == 0 || depth == MAX_DEPTH){
            node->content.push_back(object);
            // std::cout << "put sth in leaf at depth: " << depth << ", content size: " << node->content.size() << std::endl;
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
        glm::vec3 objectCenter = object->getBounds()->getCenter();
        glm::vec3 nodeCenter = node->aabb->getCenter();
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

        // std::cout << "childIndex: " << childIndex << std::endl;

        if (node->children.at(childIndex) == nullptr) {
            

            glm::vec3 childMin(0,0,0);
            glm::vec3 childMax(0,0,0);

            computeChildBound(childIndex, node->aabb->getCenter(), node->aabb->getMin(), node->aabb->getMax(), childMin, childMax);

            // switch(childIndex) {
            //     case 0:
            //         childMin = node->aabb->getMin();
            //         childMax = nodeCenter;
            //         break;
            //     case 1:
            //         childMin = node->aabb->getMin();
            //         childMin.z = nodeCenter.z;
            //         childMax = nodeCenter;
            //         childMax.z = node->aabb->getMax().z;
            //         break;
            //     case 2:
            //         childMin = node->aabb->getMin();
            //         childMin.y = nodeCenter.y;
            //         childMax = nodeCenter;
            //         childMax.y = node->aabb->getMax().y;
            //     case 3:
            //         childMin = nodeCenter;
            //         childMin.x = node->aabb->getMin().x;
            //         childMax = node->aabb->getMax();
            //         childMax.y = nodeCenter.y;
            //         break;
            //     case 4:
            //         childMin = node->aabb->getMin();
            //         childMin.x = nodeCenter.x;
            //         childMax = nodeCenter;
            //         childMax.x = node->aabb->getMax().x;
            //         break;
            //     case 5:
            //         childMin = nodeCenter;
            //         childMin.y = node->aabb->getMin().y;
            //         childMax = nodeCenter;
            //         childMax.x = node->aabb->getMax().x;
            //         childMax.z = node->aabb->getMax().z;
            //         break;
            //     case 6:
            //         childMin = nodeCenter;
            //         childMin.z = node->aabb->getMin().z;
            //         childMax = nodeCenter;
            //         childMax.x = node->aabb->getMax().x;
            //         childMax.y = node->aabb->getMax().y;
            //         break;
            //     case 7:
            //         childMin = nodeCenter;
            //         childMax = node->aabb->getMax();
            //         break;
            // }

            node->children.at(childIndex) = std::make_shared<Node>(std::make_shared<AABB>(childMin, childMax));
            node->children.at(childIndex)->depth = depth;
        }

        insert(node->children.at(childIndex), object, depth + 1);

    }
    
}