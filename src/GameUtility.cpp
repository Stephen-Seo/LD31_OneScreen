
#include <GameUtility.hpp>

#include <queue>

/*
std::unique_ptr<sf::Vector2u> GameUtility::pixelsCollidesWithColor(sf::Vector2u* coord, unsigned int size, sf::Image image, sf::Color color)
{
    sf::Vector2u imageSize = image.getSize();
    for(unsigned int i = 0; i < size; ++i)
    {
        if(coord[i].x > imageSize.x || coord[i].y > imageSize.y)
        {
            continue;
        }
        if(image.getPixel(coord[i].x, coord[i].y) == color)
        {
            return std::unique_ptr<sf::Vector2u>(new sf::Vector2u(coord[i]));
        }
    }

    return std::unique_ptr<sf::Vector2u>();
}
*/

sf::Image GameUtility::removeColorAtPixel(sf::Vector2u coord, sf::Image image, char* collisionMap)
{
    sf::Color color = image.getPixel(coord.x, coord.y);
    if(color == sf::Color::White)
    {
        return image;
    }

    image.setPixel(coord.x, coord.y, sf::Color::White);
    collisionMap[coord.x + coord.y * 720] = 0x1 | 0x2 | 0x4;

    sf::Vector2u imageSize = image.getSize();

    std::queue<sf::Vector2u> cQueue;
    cQueue.push(coord);
    sf::Vector2u temp;

    while(!cQueue.empty())
    {
        temp = cQueue.front();
        cQueue.pop();

        // check directions if same color exists
        // left
        if(temp.x > 0 && image.getPixel(temp.x - 1, temp.y) == color)
        {
            cQueue.push(sf::Vector2u(temp.x - 1, temp.y));
            image.setPixel(temp.x - 1, temp.y, sf::Color::White);
            collisionMap[temp.x - 1 + temp.y * 720] = 0x1 | 0x2 | 0x4;
        }
        // right
        if(temp.x < imageSize.x && image.getPixel(temp.x + 1, temp.y) == color)
        {
            cQueue.push(sf::Vector2u(temp.x + 1, temp.y));
            image.setPixel(temp.x + 1, temp.y, sf::Color::White);
            collisionMap[temp.x + 1 + temp.y * 720] = 0x1 | 0x2 | 0x4;
        }
        // up
        if(temp.y > 0 && image.getPixel(temp.x, temp.y - 1) == color)
        {
            cQueue.push(sf::Vector2u(temp.x, temp.y - 1));
            image.setPixel(temp.x, temp.y - 1, sf::Color::White);
            collisionMap[temp.x + (temp.y - 1) * 720] = 0x1 | 0x2 | 0x4;
        }
        // down
        if(temp.y < imageSize.y && image.getPixel(temp.x, temp.y + 1) == color)
        {
            cQueue.push(sf::Vector2u(temp.x, temp.y + 1));
            image.setPixel(temp.x, temp.y + 1, sf::Color::White);
            collisionMap[temp.x + (temp.y + 1) * 720] = 0x1 | 0x2 | 0x4;
        }
    }

    return image;
}

std::unique_ptr<sf::Vector2u> GameUtility::findColor(sf::Image image, sf::Color color)
{
    sf::Vector2u imageSize = image.getSize();
    for(unsigned int j = 0; j < imageSize.y; ++j)
    {
        for(unsigned int i = 0; i < imageSize.x; ++i)
        {
            if(image.getPixel(i, j) == color)
            {
                return std::unique_ptr<sf::Vector2u>(new sf::Vector2u(i, j));
            }
        }
    }

    return std::unique_ptr<sf::Vector2u>();
}

std::unique_ptr<sf::Vector2u[]> GameUtility::getEdges(sf::RectangleShape shape)
{
    std::unique_ptr<sf::Vector2u[]> edges(new sf::Vector2u[8]);
    sf::FloatRect globalBox = shape.getGlobalBounds();

    // corners
    // top left
    edges[0].x = (unsigned int) (globalBox.left + 0.5f);
    edges[0].y = (unsigned int) (globalBox.top + 0.5f);
    // top right
    edges[1].x = (unsigned int) (globalBox.left + globalBox.width + 0.5f);
    edges[1].y = edges[0].y;
    // bottom left
    edges[2].x = edges[0].x;
    edges[2].y = (unsigned int) (globalBox.top + globalBox.height + 0.5f);
    // bottom right
    edges[3].x = edges[1].x;
    edges[3].y = edges[2].y;

    // sides
    // left
    edges[4].x = edges[0].x;
    edges[4].y = (unsigned int) (globalBox.top + globalBox.height / 2.0f + 0.5f);
    // right
    edges[5].x = edges[3].x;
    edges[5].y = edges[4].y;
    // up
    edges[6].x = (unsigned int) (globalBox.left + globalBox.width / 2.0f + 0.5f);
    edges[6].y = edges[0].y;
    // down
    edges[7].x = edges[6].x;
    edges[7].y = edges[3].y;

    return std::move(edges);
}

bool GameUtility::isWithin(int x, int y, sf::FloatRect rect)
{
    float fx = (float) x;
    float fy = (float) y;

    return fx >= rect.left && fx <= rect.left + rect.width &&
           fy >= rect.top && fy <= rect.top + rect.height;
}
