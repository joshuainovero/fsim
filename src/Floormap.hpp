#pragma once
#include <memory>
#include "Node.hpp"
#include "Controller.hpp"

namespace fsim
{
    class Floormap
    {
    public:
        Floormap(uint32_t columns, const std::string& mapDataPath_, sf::RenderWindow* window, std::vector<Node*>* nodes_ = nullptr);
        ~Floormap();

        // Initializes vertex positions and colors of all generated nodes into one array
        void initVertexArray();

        // Sets a texture to the map sprite
        void setMapTexture(sf::Texture* texture);
        
        // Draws the map sprite
        void drawMap(sf::RenderWindow* window);

        // Copies data between node datas and node pointers
        void copy_node_data_to_node_pointers(); 
        void copy_node_pointers_to_node_data();
                
        // Getters
        sf::Vector2u clickPosition(sf::Vector2f worldPos) const;
        uint32_t     getTotalRows() const;
        uint32_t     getTotalCols() const;
        Node*        getStart() const;
        Node*        getTarget() const;
        float        getTileSize() const;

        // FOR DEBUGGING ONLY
        void saveChanges();
        void setStart(Node* node);
        void setTarget(Node* node);

    public:
        std::unique_ptr<sf::VertexArray> nodePositions; // Stores vertex positions and colors of generated nodes
        std::vector<Node*>*              nodes;         // Vector pointer of pointer nodes
        std::vector<Node*>               exitNodes;     // Stores all the exits of a floor

        sf::View mapView; // Map Camera/View

        inline static const size_t minCols = 37;  // Minimum columns
        inline static const size_t maxCols = 359; // Maximum columns
        

    private:
        std::vector<char>   nodeDatas; // Stores the data of the floor map
        uint32_t totalRows, totalCols; // Dimensions of the graph
        float    tileSize;             // Size of a single tile

        sf::Sprite  mapSprite;   // Map image or sprite
        std::string mapDataPath; // File path of texture

        Node* start;  // Start node - FOR DEBUGGING
        Node* target; // Target node - FOR DEBUGGING
    };
}