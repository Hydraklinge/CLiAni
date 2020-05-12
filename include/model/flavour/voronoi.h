//╔═══════════════════════════════════════════════════════════════════════════════════════╗
//║▒░░▒▒░░▒▒▒░░▒▒▒░░▒▒░░▒▒▒▒░░▒▒▒░░▒▒▒▒▒▒▒░░▒░░▒▒▒▒░░▒▒░░▒▒▒░░▒▒▒▒░░▒▒░░▒▒▒░░▒▒▒▒▒▒░░▒▒░░▒║
//╚═══════════════════════════════════════════════════════════════════════════════════════╝
//##############################################################################
//## Project: ClAni ########################### Created by hury on 01.05.2020 ##
//##############################################################################
//## Author: Ryan Huth(hury) ############################# License: GNU GPLv3 ##
//##############################################################################

#ifndef CLIANIMATION_VORONOI_H
#define CLIANIMATION_VORONOI_H

#include <model/flavour/flavour.h>

#include <glm/vec2.hpp>
//#include <glm/ext/matrix_double2x3.hpp>

#define VORONOI_GRID_SCALE 32
#define VORONOI_WIDTH 128
#define VORONOI_HEIGHT 256
#define VORONOI_GRID_WIDTH (VORONOI_WIDTH / VORONOI_GRID_SCALE)
#define VORONOI_GRID_HEIGHT (VORONOI_HEIGHT / VORONOI_GRID_SCALE)

#define VORONOI_CORNER_AMOUNT ((VORONOI_GRID_WIDTH+1)*(VORONOI_GRID_HEIGHT+1))
#define VORONOI_SQUARE_AMOUNT (VORONOI_GRID_HEIGHT * VORONOI_GRID_WIDTH)

#define VORONOI_MAX_FPOINT_SQUARE 3
#define VORONOI_FP_PROBABILITIES 100

namespace tasty {

/**
 * Defines a square space in a grid which originates in the bottom left corner
 * of quadrant I. With the maximum amount of feature points in its space being
 * defined by VORONOI_MAX_FPOINT_SQUARE and FP's being defined through the
 * coordinates, which originate from the bottom left corner of the square.
 *
 * @param bot_left : Coords of bottom left corner of square
 * @param fp_amount : Amount of feature points in square
 * @param f_points : Array of feature points with size VORONOI_MAX_FPOINT_SQUARE
 */
    typedef struct {
        Vec2UI bot_left; // Coords of bottom left corner of square
        int fp_amount; // Amount of feature points in square
        Vec2D f_points[VORONOI_MAX_FPOINT_SQUARE]; // Array of feature points
    } SquareEntity;

    class Voronoi : public Flavour {
    public:
        explicit Voronoi(int seed) {
            _GridCorners = new Vec2UI[_GridSize];
            _Grid = new SquareEntity[_SquareAmount];
            generate_grid();
            precompute_prob_amount(seed + 1, VORONOI_FP_PROBABILITIES);
            generate_fpoints();
        }

        Voronoi(const Voronoi &obj) { // copy constructor
            _GridCorners = new Vec2UI[_GridSize];
            *_GridCorners = *obj._GridCorners; // copy the value
            _Grid = new SquareEntity[_SquareAmount];
            *_Grid = *obj._Grid; // copy the value
        }

        ~Voronoi() override {
            delete[]_GridCorners;
            delete[]_Grid;
        }

        double generateNoise(int x, int y) override;

    private:
        Vec2UI *_GridCorners; // Starts in bottom left corner of quadrant I.
        SquareEntity *_Grid; // Square spaces and their Feature points
        int _ProbabilityAmount[VORONOI_FP_PROBABILITIES]{};

        static const int _GridSize = VORONOI_CORNER_AMOUNT;
        static const int _SquareAmount = VORONOI_SQUARE_AMOUNT;

        void generate_grid();

        void precompute_prob_amount(unsigned int seed, int max);

        void generate_fpoints();

        double closest_point(int x, int y, double minDist, int xi, int yi);


    };
}; // end of namespace tasty
#endif //CLIANIMATION_VORONOI_H
