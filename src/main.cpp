#include "application_ui.h"
#include "SDL2_gfxPrimitives.h"
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
        filledCircleRGBA(renderer, points[i].x, points[i].y, 3, 240, 240, 23, SDL_ALPHA_OPAQUE);
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

void drawPolygon(SDL_Renderer *renderer, const Polygon &polygon)
{
    //pick a random color
    int r, g, b;
    randomColor(&r, &g, &b);
    std::vector<Sint16> xCoords(polygon.getXCoords().begin(), polygon.getXCoords().end());
    std::vector<Sint16> yCoords(polygon.getYCoords().begin(), polygon.getYCoords().end());

    filledPolygonRGBA(renderer, xCoords.data(), yCoords.data(), xCoords.size(), r, g, b, SDL_ALPHA_OPAQUE);
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
    drawPoints(renderer, app.points);
    drawTriangles(renderer, app.triangles);


    //dessiner les points du diagramme de Voronoi
    for (const Coords& point : app.pointsVoronoi)
    {
        filledCircleRGBA(renderer, point.x, point.y, 3, 255, 0, 0, SDL_ALPHA_OPAQUE);
    }

    //dessiner les cercles circonscrits
    for (const Triangle& triangle : app.triangles)
    {
        float xc, yc, rsqr;
        CircumCircle(triangle.p1.x, triangle.p1.y, triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y, triangle.p3.x, triangle.p3.y,
                     &xc, &yc, &rsqr);
        circleRGBA(renderer, xc, yc, sqrt(rsqr), 30, 30, 30, SDL_ALPHA_OPAQUE);
    }

    drawSegments(renderer, app.segments);
    
    for(const Polygon& polygon : app.polygons)
    {
        drawPolygon(renderer, polygon);
    }

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


void construitPolygones(Application& app, std::vector<Polygon>& polygons)
{
    printf("construitPolygones\n");
    polygons.clear();

    for (const Triangle& triangle : app.triangles)
    {
        // Vérifier si les points du triangle appartiennent au diagramme de Voronoi
        bool p1InVoronoi = std::find(app.pointsVoronoi.begin(), app.pointsVoronoi.end(), triangle.p1) != app.pointsVoronoi.end();
        bool p2InVoronoi = std::find(app.pointsVoronoi.begin(), app.pointsVoronoi.end(), triangle.p2) != app.pointsVoronoi.end();
        bool p3InVoronoi = std::find(app.pointsVoronoi.begin(), app.pointsVoronoi.end(), triangle.p3) != app.pointsVoronoi.end();

        if (p1InVoronoi && p2InVoronoi && p3InVoronoi)
        {
            // Les trois points du triangle appartiennent au diagramme de Voronoi
            Polygon polygon;
            polygon.addPoint(triangle.p1.x, triangle.p1.y);
            polygon.addPoint(triangle.p2.x, triangle.p2.y);
            polygon.addPoint(triangle.p3.x, triangle.p3.y);

            // Recherche des triangles adjacents
            std::vector<Triangle> adjacentTriangles;

            for (const Triangle& other : app.triangles)
            {
                if (triangle.adjacent(other))
                {
                    adjacentTriangles.push_back(other);
                }
            }

            // Recherche des centres des cercles circonscrits des triangles adjacents
            for (const Triangle& adjacent : adjacentTriangles)
            {
                float xc, yc, rsqr;
                CircumCircle(
                    adjacent.p1.x, adjacent.p1.y,
                    adjacent.p1.x, adjacent.p1.y,
                    adjacent.p2.x, adjacent.p2.y,
                    adjacent.p3.x, adjacent.p3.y,
                    &xc, &yc, &rsqr
                );

                polygon.addPoint(static_cast<int>(xc), static_cast<int>(yc));
            }

            polygons.push_back(polygon);
        }
    }
    //print polygons
    for (const Polygon& polygon : polygons)
    {
        std::cout << "Polygon" << std::endl;
        for (const Coords& point : polygon.points)
        {
            std::cout << point.x << " " << point.y << std::endl;
        }
    }
}

void construitVoronoi(Application &app)
{
    // Effectuer la triangulation de Delaunay
    delaunayTriangulation(app.points, app.triangles);

    //pour chaque triangle, déterminer les triangles adjacents. Si un triangle et un autre sont adjacents, alors on construit le segment entre les centres de leurs cercles circonscrits
    for (const Triangle& triangle : app.triangles)
    {
        for (const Triangle& triangle2 : app.triangles)
        {
            if (triangle.adjacent(triangle2))
            {
                float xc, yc, rsqr;
                CircumCircle(triangle.p1.x, triangle.p1.y, triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y, triangle.p3.x, triangle.p3.y,
                             &xc, &yc, &rsqr);
                float xc2, yc2, rsqr2;
                CircumCircle(triangle2.p1.x, triangle2.p1.y, triangle2.p1.x, triangle2.p1.y, triangle2.p2.x, triangle2.p2.y, triangle2.p3.x, triangle2.p3.y,
                             &xc2, &yc2, &rsqr2);

                //si le point n'est pas dans le très grand triangle, on l'ajoute à la liste des points du diagramme de Voronoi
                if (xc < 0 || xc > app.width || yc < 0 || yc > app.height)
                    app.pointsVoronoi.push_back(Coords{xc, yc});
                if (xc2 < 0 || xc2 > app.width || yc2 < 0 || yc2 > app.height)
                    app.pointsVoronoi.push_back(Coords{xc2, yc2});

                //on vérifie que le segment n'est pas déjà dans la liste
                Segment segment{{xc, yc}, {xc2, yc2}};
                if (std::find(app.segments.begin(), app.segments.end(), segment) == app.segments.end())
                    app.segments.push_back(segment);
            }

        }
    }   
    construitPolygones(app, app.polygons);
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
