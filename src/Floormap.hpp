#pragma once
#include <memory>
#include "Node.hpp"
#include "Controller.hpp"
#include "StartingPoints.hpp"
#include "FireGraphics.hpp"
#include "Results.hpp"

namespace fsim
{
    class Floormap
    {
    public:
        Floormap(uint32_t columns, const std::string& mapDataPath_, sf::RenderWindow* window, FloorLabel floor_, std::vector<Node*>* nodes_ = nullptr);
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

        // Generates fire graphics on the fire point
        void generateFireGraphics(Node* fireSourceNode, sf::Texture* iconTexture, const float& heatFluxValue);
                
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
        std::unique_ptr<sf::VertexArray>   nodePositions;        // Stores vertex positions and colors of generated nodes
        std::vector<fsim::StartingPoints*> startingPoints;       // Stores starting point objects
        std::vector<std::vector<Node*>>    nodeObstructionsList; // 2D vector of obstruction nodes
        std::vector<FireGraphics>          fireGraphicsList;     // Stores graphic fires
        std::vector<Results>               results;              // Statistical results
        std::vector<Node*>*                nodes;                // Vector pointer of pointer nodes
        std::vector<Node*>                 exitNodes;            // Stores all the exits of a floor        

        sf::View mapView; // Map Camera/View

        inline static const size_t minCols = 37;  // Minimum columns
        inline static const size_t maxCols = 359; // Maximum columns

        uint32_t totalNodesGenerated; // Total nodes generated
        uint32_t mouseValue;          // Mouse value for camera zoom ins and outs
        

    private:
        // std::vector<char> nodeDatas; // Stores the data of the floor map
        float*             nodeDatas;

        uint32_t          totalRows; // Total rows of nodes of the board
        uint32_t          totalCols; // Total rows os columns of the board
        float             tileSize;  // Size of a single tile

        sf::Sprite  mapSprite;   // Map image or sprite
        std::string mapDataPath; // File path of texture
        FloorLabel  floor;       // floor

        Node* start;  // Start node - FOR DEBUGGING
        Node* target; // Target node - FOR DEBUGGING
    };
}