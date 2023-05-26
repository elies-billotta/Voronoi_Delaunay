#include "application_ui.h"
#include "SDL2_gfxPrimitives.h"
#include <SDL2/SDL.h>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <algorithm>
#include <set>
#include <iostream>
#include <cmath>


#define EPSILON 0.0001f

struct Coords
{
    int x, y;

    bool operator==(const Coords& other) const
    {
        return x == other.x and y == other.y;
    }

    bool operator!=(const Coords& other) const
    {
        return !(*this == other);
    }

    bool operator<(const Coords& other) const
    {
        return x < other.x && y < other.y;
    }

};

struct Segment
{
    Coords p1, p2;

    bool operator==(const Segment& other) const
    {
        return p1 == other.p1 && p2 == other.p2 || p1 == other.p2 && p2 == other.p1;
    }

    bool operator<(const Segment& other) const
    {
        return p1.x < other.p1.x && p2.x < other.p2.x || p1.x < other.p2.x && p2.x < other.p1.x;
    }

    bool operator!=(const Segment& other) const
    {
        return !(*this == other);
    }

};

struct Triangle
{
    Coords p1, p2, p3;
    bool complet=false;

    bool operator==(const Triangle& other) const {
        return (p1 == other.p1 || p1 == other.p2 || p1 == other.p3) &&
            (p2 == other.p1 || p2 == other.p2 || p2 == other.p3) &&
            (p3 == other.p1 || p3 == other.p2 || p3 == other.p3);
    }

    bool operator!=(const Triangle& other) const {
        return !(p1 == other.p1 || p1 == other.p2 || p1 == other.p3) &&
            !(p2 == other.p1 || p2 == other.p2 || p2 == other.p3) &&
            !(p3 == other.p1 || p3 == other.p2 || p3 == other.p3);
    }

    bool adjacent(Triangle triangle) const
    {
        //si deux triangles ont deux points en commun, alors ils sont adjacents
        int nbPointsCommuns = 0;
        if (p1 == triangle.p1 || p1 == triangle.p2 || p1 == triangle.p3)
            nbPointsCommuns++;
        if (p2 == triangle.p1 || p2 == triangle.p2 || p2 == triangle.p3)
            nbPointsCommuns++;
        if (p3 == triangle.p1 || p3 == triangle.p2 || p3 == triangle.p3)
            nbPointsCommuns++;
        return nbPointsCommuns == 2;
    }

    bool operator<(const Triangle& other) const
    {
        return p1.x < other.p1.x && p2.x < other.p2.x && p3.x < other.p3.x;
    }
};

struct Polygon 
{
    std::vector<Coords> points;

    std::vector<int> getXCoords() const
    {
        std::vector<int> xCoords;
        for (const Coords& point : points)
        {
            xCoords.push_back(point.x);
        }
        return xCoords;
    }

    std::vector<int> getYCoords() const
    {
        std::vector<int> yCoords;
        for (const Coords& point : points)
        {
            yCoords.push_back(point.y);
        }
        return yCoords;
    }

    void addPoint(int x, int y)
    {
        points.push_back(Coords{x, y});
    }

};

struct Application
{
    int width, height;
    Coords focus{100, 100};

    std::vector<Coords> points;
    std::vector<Segment> segments;  
    std::vector<Triangle> triangles;
    std::vector<Coords> pointsVoronoi;
    std::vector<Polygon> polygons;
};

void drawPoints(SDL_Renderer *renderer, const std::vector<Coords> &points)
{
    for (std::size_t i = 0; i < points.size(); i++)
    {
        filledCircleRGBA(renderer, points[i].x, points[i].y, 3, 0, 0, 0, SDL_ALPHA_OPAQUE);
    }
}

void drawSegments(SDL_Renderer *renderer, const std::vector<Segment> &segments)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // Définir la couleur du rendu en rouge

    for (const Segment &segment : segments)
    {
        SDL_RenderDrawLine(renderer, segment.p1.x, segment.p1.y, segment.p2.x, segment.p2.y);
    }
}

void drawTriangles(SDL_Renderer *renderer, const std::vector<Triangle> &triangles)
{
    for (const Triangle &triangle : triangles)
    {
        SDL_RenderDrawLine(renderer, triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y);
        SDL_RenderDrawLine(renderer, triangle.p2.x, triangle.p2.y, triangle.p3.x, triangle.p3.y);
        SDL_RenderDrawLine(renderer, triangle.p3.x, triangle.p3.y, triangle.p1.x, triangle.p1.y);
       // filledTrigonRGBA(renderer, triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y, triangle.p3.x, triangle.p3.y, rand() % 255, rand() % 255, rand() % 255, SDL_ALPHA_OPAQUE);
    }
}

void randomColor(int *r, int *g, int *b)
{
    *r = rand() % 255;
    *g = rand() % 255;
    *b = rand() % 255;
}

void drawPolygon(SDL_Renderer *renderer, const std::vector<Polygon> &polygons)
{
    for (const Polygon &polygon : polygons)
    {
        int r, g, b;
        randomColor(&r, &g, &b);
        std::vector<int> xCoords = polygon.getXCoords();
        std::vector<int> yCoords = polygon.getYCoords();

        // Convertir les coordonnées int en coordonnées Sint16
        std::vector<Sint16> xCoordsSint16(xCoords.begin(), xCoords.end());
        std::vector<Sint16> yCoordsSint16(yCoords.begin(), yCoords.end());
        filledPolygonRGBA(renderer, xCoordsSint16.data(), yCoordsSint16.data(), xCoordsSint16.size(), r, g, b, SDL_ALPHA_OPAQUE);
    }
}


/*
   Détermine si un point se trouve dans un cercle définit par trois points
   Retourne, par les paramètres, le centre et le rayon
*/
bool CircumCircle(
    float pX, float pY,
    float x1, float y1, float x2, float y2, float x3, float y3,
    float *xc, float *yc, float *rsqr
)
{
    float m1, m2, mx1, mx2, my1, my2;
    float dx, dy, drsqr;
    float fabsy1y2 = fabs(y1 - y2);
    float fabsy2y3 = fabs(y2 - y3);

    /* Check for coincident points */
    if (fabsy1y2 < EPSILON && fabsy2y3 < EPSILON)
        return (false);

    if (fabsy1y2 < EPSILON)
    {
        m2 = -(x3 - x2) / (y3 - y2);
        mx2 = (x2 + x3) / 2.0;
        my2 = (y2 + y3) / 2.0;
        *xc = (x2 + x1) / 2.0;
        *yc = m2 * (*xc - mx2) + my2;
    }
    else if (fabsy2y3 < EPSILON)
    {
        m1 = -(x2 - x1) / (y2 - y1);
        mx1 = (x1 + x2) / 2.0;
        my1 = (y1 + y2) / 2.0;
        *xc = (x3 + x2) / 2.0;
        *yc = m1 * (*xc - mx1) + my1;
    }
    else
    {
        m1 = -(x2 - x1) / (y2 - y1);
        m2 = -(x3 - x2) / (y3 - y2);
        mx1 = (x1 + x2) / 2.0;
        mx2 = (x2 + x3) / 2.0;
        my1 = (y1 + y2) / 2.0;
        my2 = (y2 + y3) / 2.0;
        *xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
        if (fabsy1y2 > fabsy2y3)
        {
            *yc = m1 * (*xc - mx1) + my1;
        }
        else
        {
            *yc = m2 * (*xc - mx2) + my2;
        }
    }

    dx = x2 - *xc;
    dy = y2 - *yc;
    *rsqr = dx * dx + dy * dy;

    dx = pX - *xc;
    dy = pY - *yc;
    drsqr = dx * dx + dy * dy;

    return ((drsqr - *rsqr) <= EPSILON ? true : false);
}

void draw(SDL_Renderer *renderer, const Application &app)
{
    /* Remplissez cette fonction pour faire l'affichage du jeu */
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);
    drawPolygon(renderer, app.polygons);
    drawPoints(renderer, app.points);
}

// Fonction de comparaison pour trier les points selon l'axe x
bool comparePointsX(const Coords& p1, const Coords& p2)
{
    return p1.x < p2.x;
}

void delaunayTriangulation(std::vector<Coords>& points, std::vector<Triangle>& triangles)
{
    // Trier les points selon leur coordonnée x
    std::sort(points.begin(), points.end(), comparePointsX);

    // Vider la liste existante de triangles
    triangles.clear();

    // Créer un très grand triangle au bord de l'écran
    Coords p1{-1000, -1000};
    Coords p2{500, 3000};
    Coords p3{1500, -1000};
    triangles.push_back({p1, p2, p3});

    // Pour chaque point P du repère
    for (const Coords& point : points)
    {
        std::vector<Segment> LS;

        // Pour chaque triangle T déjà créé
        for (std::vector<Triangle>::iterator i = triangles.begin(); i != triangles.end();)
        {
            const Triangle& triangle = *i;

            // Vérifier si le cercle circonscrit contient le point P
            float xc, yc, rsqr;
            if (CircumCircle(point.x, point.y, triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y, triangle.p3.x, triangle.p3.y, &xc, &yc, &rsqr))
            {
                // Récupérer les segments du triangle dans LS
                LS.push_back({triangle.p1, triangle.p2});
                LS.push_back({triangle.p2, triangle.p3});
                LS.push_back({triangle.p3, triangle.p1});

                // Enlever le triangle de la liste
                i = triangles.erase(i);
            }
            else
            {
                ++i;
            }
        }
        
        // pour chaque segment S de la liste LS faire
        // si un segment est le doublon d'un autre les virer
        for (auto _s1 = LS.begin(); _s1 != LS.end();)
        {
        bool erase_s1 = false;

        for (auto _s2 = LS.begin(); _s2 != LS.end();)
        {
            if (_s1 == _s2)
            {
            ++_s2;
            continue;
            }

            if (_s1->p1 == _s2->p2 && _s1->p2 == _s2->p1)
            {
            _s2 = LS.erase(_s2);
            erase_s1 = true;
            }
            else
            {
            ++_s2;
            }
        }

        if (erase_s1)
        {
            _s1 = LS.erase(_s1);
        }
        else
        {
            ++_s1;
        }
        }

        // pour chaque segment S de la liste LS faire
        for (const Segment& seg : LS)
        {
        // créer un nouveau triangle composé du segment S et du point P
            triangles.push_back({seg.p1, seg.p2, {point.x, point.y}});  
        }
    }   
}

void construitVoronoi(Application &app)
{
    // Effectuer la triangulation de Delaunay
    delaunayTriangulation(app.points, app.triangles);
    std::vector<Polygon> polygons;
    for (const Coords& point : app.points)
    {
        Polygon polygone;
        for (const Triangle& triangle : app.triangles)
        {
            if (triangle.p1 == point || triangle.p2 == point || triangle.p3 == point)
            {
                float xc, yc, rsqr;
                CircumCircle(triangle.p1.x, triangle.p1.y, triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y, triangle.p3.x, triangle.p3.y,
                             &xc, &yc, &rsqr);
                //app.pointsVoronoi.push_back({static_cast<int>(xc), static_cast<int>(yc)});
                polygone.addPoint((int)xc, (int)yc);
            
        }
        std::sort(polygone.points.begin(), polygone.points.end(), [](const Coords& a, const Coords& b) { return a.x < b.x;});
        app.polygons.push_back(polygone);
    }
    }
}

bool handleEvent(Application &app)
{
    /* Remplissez cette fonction pour gérer les inputs utilisateurs */
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            return false;
        else if (e.type == SDL_WINDOWEVENT_RESIZED)
        {
            app.width = e.window.data1;
            app.height = e.window.data1;
        }
        else if (e.type == SDL_MOUSEWHEEL)
        {
        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {
            if (e.button.button == SDL_BUTTON_RIGHT)
            {
                app.focus.x = e.button.x;
                app.focus.y = e.button.y;
                app.points.clear();
                app.pointsVoronoi.clear();
                app.segments.clear();
                app.polygons.clear();
                app.triangles.clear();
            }
            else if (e.button.button == SDL_BUTTON_LEFT)
            {
                app.focus.y = 0;
                app.points.push_back(Coords{e.button.x, e.button.y});
                construitVoronoi(app);
            }
        }
    }
    return true;
}

int main(int argc, char **argv)
{
    SDL_Window *gWindow;
    SDL_Renderer *renderer;
    Application app{720, 720, Coords{0, 0}};
    bool is_running = true;

    // Creation de la fenetre
    gWindow = init("Awesome Voronoi", 720, 720);

    if (!gWindow)
    {
        SDL_Log("Failed to initialize!\n");
        exit(1);
    }

    renderer = SDL_CreateRenderer(gWindow, -1, 0); // SDL_RENDERER_PRESENTVSYNC

    /*  GAME LOOP  */
    while (true)
    {
        // INPUTS
        is_running = handleEvent(app);
        if (!is_running)
            break;

        // EFFACAGE FRAME
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // DESSIN FRAME
        draw(renderer, app);

        // VALIDATION FRAME
        SDL_RenderPresent(renderer);

        // PAUSE en ms
        SDL_Delay(1000 / 30);
    }

    // Free resources and close SDL
    close(gWindow, renderer);

    return 0;
}
