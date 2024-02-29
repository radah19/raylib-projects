#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <stdio.h>

#define NULL 0
#define max(a,b) ((a)>(b) ? (a) : (b))
#define min(a,b) ((a)<(b) ? (a) : (b))

typedef struct {
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;

    int radius;
    int id;

    float mass;
    float friction;
} Ball;

typedef struct {
    Vector2 s; //Start
    Vector2 e; //End
    float radius;
} LineSegment;

void newWall(Rectangle* walls, int index, float _x, float _y, float _width, float _height) {
    walls[index].x = _x;
    walls[index].y = _y;
    walls[index].width = _width;
    walls[index].height = _height;
}

int main(void) {
    // ---------------------------- Initialization ----------------------------
    const int screenDimensions = 960;

    int ballsAmn = 0;
    int ballsArrLength = 10;
    Ball* balls = malloc(sizeof(Ball) * ballsArrLength);
    Ball* selectedBall = NULL;

    int collidingPairsLen = 10;
    int colSize = 0;
    Ball** collidingPairs = malloc(sizeof(*collidingPairs) * 2 * collidingPairsLen);

    Rectangle* walls = malloc(sizeof(Rectangle));
    int wallsAmn = 1;
    Rectangle floor = { 96, screenDimensions - 64, screenDimensions - 96 * 2, 32 };
    walls[0] = floor;

    int segmentsAmn = 3;
    float myRadius = 15.0f;
    LineSegment* segments = malloc(sizeof(LineSegment) * segmentsAmn);
    LineSegment* selectedLineSegment = NULL;

    LineSegment seg0 = { 30.0f, 30.0f, 180.0f, 30.0f, myRadius };
    segments[0] = seg0;
    LineSegment seg1 = { 30.0f, 30.0f, 180.0f, 30.0f, myRadius };
    segments[1] = seg1;
    LineSegment seg2 = { 30.0f, 30.0f, 180.0f, 30.0f, myRadius };
    segments[2] = seg2;

    int fakeBallsAmn = 0;
    int fakeBallsArrLength = 10;
    Ball** fakeBalls = malloc(sizeof(Ball) * ballsArrLength);

    int boolSelectedLineStart = 0;

    InitWindow(screenDimensions, screenDimensions, "Ball Sim 2");
    SetTargetFPS(60);

    // ---------------------------- Game Loop ----------------------------
    while (!WindowShouldClose()) {
        //Player Input
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            if (ballsAmn == ballsArrLength) {
                ballsArrLength *= 2;
                balls = realloc(balls, sizeof(Ball) * ballsArrLength);
            }

            Ball* b = &balls[ballsAmn];
            b->pos.x = GetMouseX();
            b->pos.y = GetMouseY();

            b->vel.x = 0;
            b->vel.y = 0;
            b->acc.x = 0;
            b->acc.y = 0;

            b->radius = GetRandomValue(10.0f, 20.0f);
            b->id = ballsAmn;

            b->mass = b->radius * 100.0f;
            b->friction = 0.0f;

            ballsAmn++;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && selectedBall == NULL && selectedLineSegment == NULL) {

            if (selectedBall == NULL) {
                for (int i = 0; i < ballsAmn; i++) {
                    if (CheckCollisionPointCircle(GetMousePosition(), balls[i].pos, balls[i].radius)) {
                        selectedBall = &balls[i];
                    }
                }
            }

            if (selectedLineSegment == NULL) {
                for (int i = 0; i < segmentsAmn; i++) {
                    LineSegment* seg = &segments[i];

                    if (CheckCollisionPointCircle(GetMousePosition(), seg->s, myRadius)) {
                        selectedLineSegment = seg;
                        boolSelectedLineStart = true;
                    }

                    if (CheckCollisionPointCircle(GetMousePosition(), seg->e, myRadius)) {
                        selectedLineSegment = seg;
                        boolSelectedLineStart = false;
                    }
                }
            }
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            selectedBall = NULL;
            selectedLineSegment = NULL;
        }

        //Physics Sim
        for (int i = 0; i < ballsAmn; i++) {
            Ball* b1 = &balls[i];

            if (selectedBall != NULL && selectedBall == b1) {
                b1->pos.x = GetMouseX();
                b1->pos.y = GetMouseY();

                b1->vel.x = GetMouseDelta().x;
                b1->vel.y = GetMouseDelta().y;
            }
            else {
                b1->acc.x = -(b1->vel.x * 0.05f);
                b1->acc.y = -(b1->vel.y * 0.05f) + 2 * GetFrameTime() * 60;

                b1->vel.x += b1->acc.x;
                b1->vel.y += b1->acc.y;

                b1->pos.x += b1->vel.x;
                b1->pos.y += b1->vel.y;

                if (fabsf(b1->vel.x * b1->vel.x + b1->vel.y * b1->vel.y) <= 0.000000001f) {
                    b1->vel.x = 0;
                    b1->vel.y = 0;
                }
            }

            // Static Collisions
            for (int j = 0; j < ballsAmn; j++) {
                //Collision with ball
                Ball* b2 = &balls[j];

                if (b1->id != b2->id) {
                    if (CheckCollisionCircles(b1->pos, b1->radius, b2->pos, b2->radius)) {
                        // Add to Colliding Pairs
                        if (colSize == collidingPairsLen) {
                            collidingPairsLen *= 2;
                            collidingPairs = realloc(collidingPairs, sizeof(*collidingPairs) * 2 * collidingPairsLen);
                        }

                        collidingPairs[colSize] = b1;
                        collidingPairs[colSize + 1] = b2;
                        colSize += 2;

                        //Distance between Ball Centers
                        float fDist = sqrtf((b1->pos.x - b2->pos.x) * (b1->pos.x - b2->pos.x)
                            + (b1->pos.y - b2->pos.y) * (b1->pos.y - b2->pos.y));
                        float fOverlap = (fDist - b1->radius - b2->radius) * 0.5f;

                        //Displacement
                        b1->pos.x -= fOverlap * (b1->pos.x - b2->pos.x) / fDist;
                        b1->pos.y -= fOverlap * (b1->pos.y - b2->pos.y) / fDist;
                        b2->pos.x += fOverlap * (b1->pos.x - b2->pos.x) / fDist;
                        b2->pos.y += fOverlap * (b1->pos.y - b2->pos.y) / fDist;
                    }
                }

                // Edge Collisions
                for (int i = 0; i < segmentsAmn; i++) {
                    LineSegment* seg = &segments[i];

                    //Define Line Region between start point and ball
                    float fLineX1 = seg->e.x - seg->s.x;
                    float fLineY1 = seg->e.y - seg->s.y;

                    float fLineX2 = b1->pos.x - seg->s.x;
                    float fLineY2 = b1->pos.y - seg->s.y;

                    float fEdgeLength = fLineX1 * fLineX1 + fLineY1 * fLineY1;

                    // Our tangential value is the dot product between the line segment 
                    // and line between the origin and ball, normalised then 
                    // clamped together and divided by the line length
                    float t = max(0, min(fEdgeLength, (fLineX1 * fLineX2 + fLineY1 * fLineY2))) / fEdgeLength;

                    //Find closest points
                    float fClosestPointX = seg->s.x + t * fLineX1;
                    float fClosestPointY = seg->s.y + t * fLineY1;

                    //Calc distance between closest point and our ball
                    float fDistance = sqrtf((b1->pos.x - fClosestPointX) * (b1->pos.x - fClosestPointX) + (b1->pos.y - fClosestPointY) * (b1->pos.y - fClosestPointY));

                    //We'll know a collision occurred when
                    //the distance is less than their total radius
                    //ie there is an overlap
                    if (fDistance <= b1->radius + myRadius) {
                        //Fake collision with another ball at the closest point
                        Ball* fakeBall = malloc(sizeof(Ball));

                        fakeBall->radius = myRadius;
                        fakeBall->mass = b1->mass * 0.8f;
                        fakeBall->pos.x = fClosestPointX;
                        fakeBall->pos.y = fClosestPointY;
                        fakeBall->vel.x = -(b1->vel.x);
                        fakeBall->vel.y = -(b1->vel.y);

                        //So that we can clean later
                        if (fakeBallsAmn == fakeBallsArrLength) {
                            fakeBallsArrLength *= 2;
                            fakeBalls = realloc(fakeBalls, sizeof(*fakeBalls) * fakeBallsArrLength);
                        }

                        fakeBalls[fakeBallsAmn] = fakeBall;
                        fakeBallsAmn++;

                        // Add to Colliding Pairs
                        if (colSize == collidingPairsLen) {
                            collidingPairsLen *= 2;
                            collidingPairs = realloc(collidingPairs, sizeof(*collidingPairs) * 2 * collidingPairsLen);
                        }

                        collidingPairs[colSize] = b1;
                        collidingPairs[colSize + 1] = fakeBall;
                        colSize += 2;

                        //Displacement
                        float fOverlap = 1.0f * (fDistance - b1->radius - fakeBall->radius);

                        b1->pos.x -= fOverlap * (b1->pos.x - fakeBall->pos.x) / fDistance;
                        b1->pos.y -= fOverlap * (b1->pos.y - fakeBall->pos.y) / fDistance;
                    }
                }

                //Collision with walls
                //for (int j = 0; j < wallsAmn; j++) {
                //    if (CheckCollisionCircleRec(b1->pos, b1->radius, walls[j])) {
                //        //Displacement 
                //        while (CheckCollisionCircleRec(b1->pos, b1->radius, walls[j]))
                //            b1->pos.y -= 0.025;
                //        b1->vel.y *= -0.5;
                //    }
                //}

            }

            //Clamp Vals to Zero
            if (abs(b1->vel.x + b1->vel.y) < 0.01f) {
                b1->vel.x = 0;
                b1->vel.y = 0;
            }

            //Loop Balls
            if (b1->pos.x < -10) {
                b1->pos.x = screenDimensions + 10;
            }
            if (b1->pos.x > screenDimensions + 10) {
                b1->pos.x = -10;
            }
            if (b1->pos.y < -10) {
                b1->pos.y = screenDimensions + 10;
            }
            if (b1->pos.y > screenDimensions + 10) {
                b1->pos.y = -10;
            }
        }

        //Editing Line Segment Positions
        for (int i = 0; i < segmentsAmn; i++) {
            if (selectedLineSegment != NULL && selectedLineSegment == &segments[i]) {
                if (boolSelectedLineStart) {
                    segments[i].s = GetMousePosition();
                }
                else {
                    segments[i].e = GetMousePosition();
                }
            }
        }

        // Dynamic Collisions
        for (int i = 0; i < colSize; i += 2) {
            Ball* b1 = collidingPairs[i];
            Ball* b2 = collidingPairs[i + 1];

            float fDist = sqrtf((b1->pos.x - b2->pos.x) * (b1->pos.x - b2->pos.x)
                + (b1->pos.y - b2->pos.y) * (b1->pos.y - b2->pos.y));

            //Get Normal
            float nx = (b2->pos.x - b1->pos.x) / fDist;
            float ny = (b2->pos.y - b1->pos.y) / fDist;

            //Tangent
            float tx = -ny;
            float ty = nx;

            // Dot Product Tangent - How much velocity will be transferred to the tangent?
            float dpTan1 = b1->vel.x * tx + b1->vel.y * ty;
            float dpTan2 = b2->vel.x * tx + b2->vel.y * ty;

            // Dot Product Normal - How much velocity will be transferred to the normal?
            float dpNorm1 = b1->vel.x * nx + b1->vel.y * ny;
            float dpNorm2 = b2->vel.x * nx + b2->vel.y * ny;

            //Conservation of Momentum in 1D
            float m1 = (dpNorm1 * (b1->mass - b2->mass) + 2.0f * b2->mass * dpNorm2) / (b1->mass + b2->mass);
            float m2 = (dpNorm2 * (b2->mass - b1->mass) + 2.0f * b1->mass * dpNorm1) / (b1->mass + b2->mass);

            b1->vel.x = tx * dpTan1 + nx * m1;
            b1->vel.y = ty * dpTan1 + ny * m1;
            b2->vel.x = tx * dpTan2 + nx * m2;
            b2->vel.y = ty * dpTan2 + ny * m2;
        }

        // ---------------------------- Drawing ----------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < ballsAmn; i++) {
            DrawCircle(balls[i].pos.x, balls[i].pos.y, balls[i].radius, RED);

            DrawLine(balls[i].pos.x, balls[i].pos.y, balls[i].pos.x + balls[i].vel.x, balls[i].pos.y + balls[i].vel.y, YELLOW);
        }

        //for (int i = 0; i < wallsAmn; i++) {
        //    DrawRectangle(walls[i].x, walls[i].y, walls[i].width, walls[i].height, GRAY);
        //}

        for (int i = 0; i < segmentsAmn; i++) {
            LineSegment* seg = &segments[i];

            DrawCircle(seg->s.x, seg->s.y, myRadius, WHITE);
            DrawCircle(seg->e.x, seg->e.y, myRadius, WHITE);

            //Calc Normal
            float nx = -(seg->e.y - seg->s.y);
            float ny = (seg->e.x - seg->s.x);
            float len = sqrt(nx * nx + ny * ny);
            // Normalise & Convert to Unit Vector
            nx /= len;
            ny /= len;

            DrawLine(
                seg->s.x + nx * myRadius,
                seg->s.y + ny * myRadius,
                seg->e.x + nx * myRadius,
                seg->e.y + ny * myRadius,
                WHITE
            );
            DrawLine(
                seg->s.x - nx * myRadius,
                seg->s.y - ny * myRadius,
                seg->e.x - nx * myRadius,
                seg->e.y - ny * myRadius,
                WHITE
            );
        }

        for (int i = 0; i < colSize; i += 2) {
            DrawLineEx(collidingPairs[i]->pos, collidingPairs[i + 1]->pos, 1, GREEN);
        }

        EndDrawing();

        // ---------------------------- Cleanup ----------------------------
        collidingPairsLen = 10;
        collidingPairs = malloc(sizeof(int) * 2 * collidingPairsLen);
        colSize = 0;

        fakeBallsAmn = 0;
        fakeBallsArrLength = 10;
        fakeBalls = malloc(sizeof(Ball) * ballsArrLength);
    }

    free(collidingPairs);
    free(segments);
    free(balls);
    CloseWindow();
    return 0;
}