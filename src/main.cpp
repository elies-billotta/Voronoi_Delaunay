#include "application_ui.h"
#include "SDL2_gfxPrimitives.h"
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <algorithm>
#include <unordered_set>

#define EPSILON 0.0001f

struct Coords
{
    int x, y;

    bool operator==(const Coords& other) const
    {
        return x == other.x and y == other.y;
    }

};

struct Segment
{
    Coords p1, p2;

    bool operator==(const Segment& other) const
    {
        return p1 == other.p1 && p2 == other.p2;
    }

    bool operator<(const Segment& other) const
    {
        return p1.x < other.p1.x && p2.x < other.p2.x;
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
};

struct Polygon 
{
    std::vector<Segment> segments;

    //get the X coordinates of the points of the polygon
    std::vector<int> getXCoords() const
    {
        std::vector<int> xCoords;
        for (const Segment& segment : segments)
        {
            xCoords.push_back(segment.p1.x);
            xCoords.push_back(segment.p2.x);
        }
        return xCoords;
    }

    //get the Y coordinates of the points of the polygon
    std::vector<int> getYCoords() const
    {
        std::vector<int> yCoords;
        for (const Segment& segment : segments)
        {
            yCoords.push_back(segment.p1.y);
            yCoords.push_back(segment.p2.y);
        }
        return yCoords;
    }

};

struct Application
{
    int width, height;
    Coords focus{100, 100};

    std::vector<Coords> points;
    std::vector<Segment> segments;  
    std::vector<Triangle> triangles;
    std::vector<Polygon> polygonsVoronoi;
};

bool compareCoords(const Coords p1, const Coords p2)
{
    if (p1.y == p2.y)
        return p1.x < p2.x;
    return p1.y < p2.y;
}

void drawPoints(SDL_Renderer *renderer, const std::vector<Coords> &points)
{
    for (std::size_t i = 0; i < points.size(); i++)
    {
        filledCircleRGBA(renderer, points[i].x, points[i].y, 3, 240, 240, 23, SDL_ALPHA_OPAQUE);
    }
}

void drawSegments(SDL_Renderer *renderer, const std::vector<Segment> &segments)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE); // Définir la couleur du rendu en rouge

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
        //filledTrigonRGBA(renderer, triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y, triangle.p3.x, triangle.p3.y, rand() % 255, rand() % 255, rand() % 255, SDL_ALPHA_OPAQUE);
    }
}

void drawPolygon(SDL_Renderer *renderer, const Polygon &polygon)
{
    std::vector<Sint16> xCoords(polygon.getXCoords().begin(), polygon.getXCoords().end());
    std::vector<Sint16> yCoords(polygon.getYCoords().begin(), polygon.getYCoords().end());

    filledPolygonRGBA(renderer, xCoords.data(), yCoords.data(), xCoords.size(), rand() % 255, rand() % 255, rand() % 255, SDL_ALPHA_OPAQUE);
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
    //drawTriangles(renderer, app.triangles);

    //dessiner le centre des cercles circonscrits
    for (const Triangle& triangle : app.triangles)
    {
        float xc, yc, rsqr;
        CircumCircle(triangle.p1.x, triangle.p1.y, triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y, triangle.p3.x, triangle.p3.y,
                     &xc, &yc, &rsqr);
        filledCircleRGBA(renderer, xc, yc, 3, 255, 0, 0, SDL_ALPHA_OPAQUE);
    }

    //dessiner les cercles circonscrits
    for (const Triangle& triangle : app.triangles)
    {
        float xc, yc, rsqr;
        CircumCircle(triangle.p1.x, triangle.p1.y, triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y, triangle.p3.x, triangle.p3.y,
                     &xc, &yc, &rsqr);
        circleRGBA(renderer, xc, yc, sqrt(rsqr), 30, 30, 30, SDL_ALPHA_OPAQUE);
    }

    //dessiner les segments des polygones de voronoi

    for (const Polygon& polygon : app.polygonsVoronoi)
    {
        drawPolygon(renderer, polygon);
    }

    drawSegments(renderer, app.segments);

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

    // Créer un très grand triangle
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

        std::vector<Segment> uniqueSegments;

        // Supprimer les doublons dans LS
        for (const Segment& segment : LS)
        {
            if (std::find(uniqueSegments.begin(), uniqueSegments.end(), segment) == uniqueSegments.end())
            {
                uniqueSegments.push_back(segment);
            }
        }

        // Créer de nouveaux triangles avec les segments et le point P
        for (const Segment& segment : uniqueSegments)
        {
            triangles.push_back({segment.p1, segment.p2, point});
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

               /* //si l'un des deux point appartient à un polygone, on rajoute le segment au polygone
                bool segmentDejaPresent = false;
                for (Polygon& polygon : app.polygonsVoronoi)
                {
                    if (polygon.segments.size() > 0)
                    {
                        if (polygon.segments[0].p1 == triangle.p1 || polygon.segments[0].p1 == triangle.p2 || polygon.segments[0].p1 == triangle.p3)
                        {
                            polygon.segments.push_back({{xc, yc}, {xc2, yc2}});
                            segmentDejaPresent = true;
                        }
                        else if (polygon.segments[0].p1 == triangle2.p1 || polygon.segments[0].p1 == triangle2.p2 || polygon.segments[0].p1 == triangle2.p3)
                        {
                            polygon.segments.push_back({{xc, yc}, {xc2, yc2}});
                            segmentDejaPresent = true;
                        }
                    }
                    else
                    {
                        polygon.segments.push_back({{xc, yc}, {xc2, yc2}});
                        segmentDejaPresent = true;
                    }
                }*/

                app.segments.push_back({{xc, yc}, {xc2, yc2}});
            }
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
