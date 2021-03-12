#include <SFML/Graphics.hpp>
#include <iostream>

class Conway
{
public:
    Conway(int width, int height, int scale, int offset);
    ~Conway();
    void updateCells();
    void draw(sf::RenderTarget& target) const;

private:
    int m_rows;
    int m_scale;
    int m_offset;
    int m_columns;
    unsigned char **m_cellArray;
    unsigned char **m_newCellArray;
};

// Constructor
Conway::Conway(int width, int height, int scale, int offset)
{
    // Rows and columns of the grid
    m_columns = height / scale;
    m_rows = width / scale;

    // Space between tiles
    m_offset = offset;

    // Size of each tile
    m_scale = scale;

    // Allocate space for required rows
    m_cellArray = new unsigned char*[m_rows];
    m_newCellArray = new unsigned char*[m_rows];

    // Allocate space for columns
    for (int i = 0; i < m_rows; i++)
    {
        m_cellArray[i] = new unsigned char[m_columns];
        m_newCellArray[i] = new unsigned char[m_columns];
    }

    // Initialize random seed
    srand(time(nullptr));

    // Populate the 2d array with values between 0 and 1
    for (int i = 0; i < m_rows; ++i)
        for (int j = 0; j < m_columns; ++j)
            m_cellArray[i][j] = rand() & 1;
}

// Destructor
Conway::~Conway()
{
    // Delete the space allocated for each cell
    for (int i = 0; i < m_rows; i++)
    {
        delete[] m_cellArray[i];
        delete[] m_newCellArray[i];
    }

    // Delete the array fully
    delete[] m_cellArray;
    delete[] m_newCellArray;
}

// Update the cells
void Conway::updateCells()
{
    // Go over each cell and update its existence
    for (int x = 0; x < m_rows; x++)
        for (int y = 0; y < m_columns; y++)
        {
            // Get the neighbours of the cell
            int neighbours = 0;

            // Check surrounding neighbours of the cell for live cells
            for (int i = -1; i < 2; i++)
                for (int j = -1; j < 2; j++)
                    neighbours += m_cellArray[(x + i + m_rows) % m_rows][(y + j + m_columns) % m_columns];

            // If the current cell is alive, subtract it from the alive neighbours
            neighbours -= m_cellArray[x][y];

            // Inlined simplified evaluation without if statement
            m_newCellArray[x][y] = neighbours == 3 || (neighbours == 2 && m_cellArray[x][y]) ? 1 : 0;
        }

    // Copy the new array to the old array
    std::swap(m_newCellArray, m_cellArray);
};

// Draw all the cell's to the screen
void Conway::draw(sf::RenderTarget &target) const
{
    // Go over each cell and draw it to the screen
    for (int x = 0; x < m_rows; x++)
        for (int y = 0; y < m_columns; y++)
        {
            // Create a cell of a specified size
            sf::RectangleShape cell(sf::Vector2f(m_scale - m_offset, m_scale - m_offset));
            cell.setPosition(static_cast<float>(x * m_scale), static_cast<float>(y * m_scale));

            // Set the cell's color based if its' alive or dead
            m_cellArray[x][y] == 1 ? cell.setFillColor(sf::Color::Blue) : cell.setFillColor(sf::Color::White);

            // Draw the cell onto the screen
            target.draw(cell);
        }
}

int main()
{
    // Create the window
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Conway's Game of Life");

    // Conway's Game of Life object
    Conway conway(1280, 720, 15, 1);

    // Set the framerate limit
    window.setFramerateLimit(30);

    // Run while the window is open
    while (window.isOpen())
    {
        // Process events
        sf::Event event{};
        while (window.pollEvent(event))
        {
            // Close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Update the logic
        conway.updateCells();

        // Clear the window with a black color
        window.clear(sf::Color::Black);

        // Draw all the cells to the window
        conway.draw(window);

        // End the current frame
        window.display();
    }
}
