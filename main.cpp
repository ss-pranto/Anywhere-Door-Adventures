#include <windows.h>
#include <GL/glut.h>
#include <cmath>

float cloudPosition = -1.0f;
float cloudPosition2 = -0.6f;
float cloudPosition3 = -1.2f;
float cloudPosition4 = 1.2f;
float cloudPosition5 = -1.2f;
float cloudPosition6 = 1.2f;

GLfloat position = 0.0f;
GLfloat speed = 1.0f;

float doorScale = 1.0f;
float doorScaleAngle = 0.0f;

float sunBaseY = 900.0f;      // original Y position of sun
float sunYOffset = 0.0f;      // animated offset
float sunAngle = 0.0f;        // angle to animate sin wave

float humanX = -20.0f;           // Start far left
const float targetX = 350.0f;     // Final horizontal position
bool moveHuman = true;           // Control animation flag

bool showDoor = false;
bool scaleHum = false;


float person0X = 800.0f, person0Y = 800.0f;
float person1X = 500.0f, person1Y = 800.0f;
float person2X = 650.0f, person2Y = 800.0f;
float person3X = -20.0f, person3Y = 800.0f;

float person0Scale = 1.0f;
float person1Scale = 1.0f;
float person2Scale = 1.0f;
float person3Scale = 1.0f;




struct SimpleBubble {
    float x, y;
    const char* message;
    int len;
};

SimpleBubble fixedBubbles[] = {
    {350, 470, "What's up, bro?", 40},
    {350, 470, "How things going", 150},
    {480, 470, "Everything good bro.", 300},
    {580, 470, "From tomorrow our sem break starts", 370},
    {350, 470, "Yes, The finals were tough", 250},
    {480, 470, "Finally, some rest time!", 230},
    {350, 470, "Wait... what's that?", 160},
    {350, 470, "A door just appeared!", 190},
    {580, 470, "Where did it come from?", 220},
    {480, 470, "Looks mysterious...", 200},
    {350, 470, "Should we check it out?", 220},
    {350, 470, "I'm kind of excited!", 170},
    {480, 470, "Let’s see what’s inside.", 200},
    {580, 470, "Be careful, though.", 160},
    {480, 470, "Alright, let's go together.", 220},
    {480, 470, "Ahhh......", 100}
};

const int fixedBubbleCount = sizeof(fixedBubbles) / sizeof(fixedBubbles[0]);

int currentBubbleIndex = 0;
bool showBubbles = false;
bool bubblesDone = false;




void initGL() {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
}

void setColor255(int r, int g, int b) {
    glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);
}

void drawCircle(float x, float y, float radius) {
    radius += 0.3;
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * 3.1415926f * i / 100;
        float dx = radius * cosf(angle);
        float dy = radius * sinf(angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

void drawsquad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glVertex2f(x4, y4);
    glEnd();
}

void drawCloud(float baseX, float baseY, float scale = 60.0f) {
    glColor3f(1.0f, 1.0f, 1.0f);  // White color for cloud

    drawCircle(baseX - scale * 0.6f, baseY, scale * 0.4f);  // Left puff
    drawCircle(baseX, baseY, scale * 0.6f);                 // Center puff
    drawCircle(baseX + scale * 0.6f, baseY, scale * 0.4f);  // Right puff
}

void drawTree(float x, float y) {
    // Leaf clusters (relative to x, y)
    setColor255(42, 107, 68);
    drawCircle(x + 100.0, y + 430.0, 110.0);      // Left leaf
    setColor255(88, 139, 66);
    drawCircle(x + 190.0, y + 310.0, 110.0);      // Top leaf
    drawCircle(x + 280.0, y + 430.0, 110.0);      // Right leaf

    // Tree trunk
    setColor255(139, 87, 29);
    drawsquad(x + 165.0, y + 355.0, x + 165.0, y + 735.0,
              x + 195.0, y + 735.0, x + 195.0, y + 355.0);

    // Branch extension or bark detail
    drawsquad(x + 190.0, y + 436.0, x + 190.0, y + 465.0,
              x + 266.0, y + 424.0, x + 260.0, y + 420.0);
}

void drawDoor(float x, float y) {
    // Main double door panels
    setColor255(236, 96, 37);
    drawsquad(x + 1125.0, y + 500.0, x + 1125.0, y + 880.0, x + 1150.0, y + 880.0, x + 1150.0, y + 500.0);
    drawsquad(x + 1300.0, y + 500.0, x + 1300.0, y + 880.0, x + 1325.0, y + 880.0, x + 1325.0, y + 500.0);

    drawsquad(x + 1150.0, y + 500.0, x + 1150.0, y + 525.0, x + 1300.0, y + 525.0, x + 1300.0, y + 500.0);

    setColor255(164, 49, 23);
    drawsquad(x + 1125.0, y + 880.0, x + 1125.0, y + 905.0, x + 1325.0, y + 905.0, x + 1325.0, y + 880.0);

    setColor255(247, 151, 59);
    drawsquad(x + 1325.0, y + 500.0, x + 1325.0, y + 905.0, x + 1350.0, y + 905.0, x + 1350.0, y + 500.0);

    setColor255(255, 204, 70);
    drawsquad(x + 1150.0, y + 525.0, x + 1150.0, y + 880.0, x + 1200.0, y + 840.0, x + 1200.0, y + 560.0);
}


    void drawHuman(float baseX, float baseY, int shirtR, int shirtG, int shirtB, int pantR, int pantG, int pantB) {
        // HEAD
        setColor255(255, 224, 189);
        drawCircle(baseX, baseY - 240, 35);

        // NECK
        setColor255(255, 224, 189);
        drawsquad(baseX - 5, baseY - 205, baseX - 5, baseY - 195, baseX + 5, baseY - 195, baseX + 5, baseY - 205);

        // BODY / TORSO
        setColor255(shirtR, shirtG, shirtB);  // custom shirt color
        drawsquad(baseX - 25, baseY - 195, baseX - 25, baseY - 80, baseX + 25, baseY - 80, baseX + 25, baseY - 195);

        // ARMS
        setColor255(255, 224, 189);  // skin
        // Left upper arm
        drawsquad(baseX - 25, baseY - 190, baseX - 55, baseY - 160, baseX - 50, baseY - 155, baseX - 20, baseY - 185);
        // Right upper arm
        drawsquad(baseX + 25, baseY - 190, baseX + 55, baseY - 160, baseX + 50, baseY - 155, baseX + 20, baseY - 185);
        // Left lower arm
        drawsquad(baseX - 55, baseY - 160, baseX - 60, baseY - 120, baseX - 55, baseY - 115, baseX - 50, baseY - 155);
        // Right lower arm
        drawsquad(baseX + 55, baseY - 160, baseX + 60, baseY - 120, baseX + 55, baseY - 115, baseX + 50, baseY - 155);

        // HANDS
        drawCircle(baseX - 60, baseY - 120, 6);  // Left hand
        drawCircle(baseX + 60, baseY - 120, 6);  // Right hand

        // LEGS
        setColor255(pantR, pantG, pantB);  // custom pants color
        drawsquad(baseX - 20, baseY - 80, baseX - 20, baseY + 60, baseX - 5, baseY + 60, baseX - 5, baseY - 80); // Left leg
        drawsquad(baseX + 5, baseY - 80, baseX + 5, baseY + 60, baseX + 20, baseY + 60, baseX + 20, baseY - 80); // Right leg


    }

    void drawFlatHumanFacingRight(float baseX, float baseY) {
        // HEAD
        setColor255(255, 224, 189);  // skin color
        drawCircle(baseX, baseY - 240, 30);

        // NECK
        setColor255(255, 224, 189);
        drawsquad(baseX - 5, baseY - 210, baseX - 5, baseY - 200, baseX + 5, baseY - 200, baseX + 5, baseY - 210);

        // BODY
        setColor255(230, 60, 20);  // red-orange shirt
        drawsquad(baseX - 25, baseY - 200, baseX - 25, baseY - 80, baseX + 25, baseY - 80, baseX + 25, baseY - 200);

        // RIGHT ARM (raised)
        setColor255(230, 60, 20);
        drawsquad(baseX + 25, baseY - 190, baseX + 60, baseY - 230, baseX + 70, baseY - 220, baseX + 35, baseY - 180);

        // RIGHT HAND
        setColor255(255, 224, 189);
        drawCircle(baseX + 70, baseY - 230, 6);

        // LEFT ARM (down)
        setColor255(230, 60, 20);
        drawsquad(baseX - 25, baseY - 190, baseX - 35, baseY - 120, baseX - 25, baseY - 120, baseX - 15, baseY - 190);

        // LEFT HAND
        setColor255(255, 224, 189);
        drawCircle(baseX - 30, baseY - 120, 6);

        // LEGS
        setColor255(10, 20, 40);  // navy pants
        drawsquad(baseX - 20, baseY - 80, baseX - 20, baseY + 60, baseX - 5, baseY + 60, baseX - 5, baseY - 80); // left leg
        drawsquad(baseX + 5, baseY - 80, baseX + 5, baseY + 60, baseX + 20, baseY + 60, baseX + 20, baseY - 80); // right leg

        // FEET
        setColor255(40, 30, 20);
        drawsquad(baseX - 20, baseY + 60, baseX - 20, baseY + 70, baseX - 5, baseY + 70, baseX - 5, baseY + 60);
        drawsquad(baseX + 5, baseY + 60, baseX + 5, baseY + 70, baseX + 20, baseY + 70, baseX + 20, baseY + 60);
    }



    void drawGrass(float x, float y, float scale = 1.0f) {
        setColor255(41, 104, 64); // Grass green

        // Center blade
        drawTriangle(x, y,
                     x + 5 * scale, y - 15 * scale,
                     x + 10 * scale, y);

        // Left blade
        drawTriangle(x - 10 * scale, y,
                     x - 5 * scale, y - 10 * scale,
                     x, y);

        // Right blade
        drawTriangle(x + 10 * scale, y,
                     x + 15 * scale, y - 10 * scale,
                     x + 20 * scale, y);
    }


    void drawOval(float x, float y, float radiusX, float radiusY) {
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i <= 100; i++) {
            float angle = 2.0f * 3.1415926f * i / 100;
            float dx = radiusX * cosf(angle);
            float dy = radiusY * sinf(angle);
            glVertex2f(x + dx, y + dy);
        }
        glEnd();
    }


    void drawSun(float centerX, float centerY, float radius, int rayCount = 12, float rayLength = 20.0f) {
        // Sun core
        setColor255(255, 204, 0); // bright yellow
        drawCircle(centerX, centerY, radius);

        // Sun rays (triangles around the circle)
        setColor255(255, 153, 0); // orange for rays
        for (int i = 0; i < rayCount; i++) {
            float angle = 2.0f * 3.1415926f * i / rayCount;
            float dx = cosf(angle);
            float dy = sinf(angle);

            // Triangle base (just outside the sun)
            float baseX1 = centerX + dx * (radius + 2);
            float baseY1 = centerY + dy * (radius + 2);

            float baseX2 = centerX + dx * (radius + 2) + dy * 5;
            float baseY2 = centerY + dy * (radius + 2) - dx * 5;

            float baseX3 = centerX + dx * (radius + 2) - dy * 5;
            float baseY3 = centerY + dy * (radius + 2) + dx * 5;

            float tipX = centerX + dx * (radius + rayLength);
            float tipY = centerY + dy * (radius + rayLength);

            drawTriangle(baseX2, baseY2, baseX3, baseY3, tipX, tipY);
        }
    }




    void drawSpeechBubble(float x, float y, const char* message, int len) {
        int textLength = len;
        for (int i = 0; message[i] != '\0'; i++) {
            textLength += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, message[i]);
        }

        float padding = 50.0f;
        float bubbleWidth = textLength + 2 * padding;
        float bubbleHeight = 40.0f;

        // Draw bubble background
        glColor3f(1.0f, 1.0f, 1.0f); // white
        glBegin(GL_POLYGON);
            glVertex2f(x, y);
            glVertex2f(x + bubbleWidth, y);
            glVertex2f(x + bubbleWidth, y + bubbleHeight);
            glVertex2f(x + 20, y + bubbleHeight);
            glVertex2f(x + 10, y + bubbleHeight + 10); // tail
            glVertex2f(x + 10, y + bubbleHeight);
            glVertex2f(x, y + bubbleHeight);
        glEnd();

        // Border (optional)
        glColor3f(0.0f, 0.0f, 0.0f); // black
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(x, y);
            glVertex2f(x + bubbleWidth, y);
            glVertex2f(x + bubbleWidth, y + bubbleHeight);
            glVertex2f(x + 20, y + bubbleHeight);
            glVertex2f(x + 10, y + bubbleHeight + 10);
            glVertex2f(x + 10, y + bubbleHeight);
            glVertex2f(x, y + bubbleHeight);
        glEnd();

        // Text
        glColor3f(0.0f, 0.0f, 0.0f); // black
        glRasterPos2f(x + padding - 20, y + 32);
        for (int i = 0; message[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
        }
    }





void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    setColor255(237, 185, 129);
    glTranslatef(0.0f, 0.0f, -0.1f);
    glBegin(GL_QUADS);
        glVertex2f(0.0,810.0);
        glVertex2f(0.0,1040.0);
        glVertex2f(1536.0,1040.0);
        glVertex2f(1536.0,810.0);
    glEnd();



    drawSun(300, 200+ sunYOffset, 70, 16, 25);



    float cloudX = 750 + cloudPosition * 600;
    drawCloud(cloudX, 200,30);
    float cloudX2 = 500 + cloudPosition2 * 600;
    drawCloud(cloudX2, 100,100);
    float cloudX3 = 1000 - cloudPosition3 * 600;
    drawCloud(cloudX3, 110,80);
    float cloudX4 = 600 - cloudPosition4 * 600;
    drawCloud(cloudX4, 150,30);
    float cloudX5 = 1200 - cloudPosition5 * 600;
    drawCloud(cloudX5, 70,120);
    float cloudX6 = 200 - cloudPosition6 * 600;
    drawCloud(cloudX6, 190,60);


    //c building
    setColor255(135, 173, 179);
    drawCircle(1200.0, 420.0, 250.0);
    setColor255(70, 130, 180);
    drawsquad(950.0,400.0, 950.0,460.0, 1450.0,460.0, 1450.0,400.0);
    drawsquad(980.0,300.0, 950.0,360.0, 1450.0,360.0, 1420.0,300.0);
    drawsquad(980.0,500.0, 950.0,560.0, 1410.0,560.0, 1440.0,500.0);



    //d building
    setColor255(163, 206, 214);
    drawsquad(360.0,330.0, 360.0,660.0, 1185.0,660.0, 1185.0,330.0);

    // Windows
    setColor255(70, 130, 180);
    float startX = 400.0;
    float startY = 360.0;
    float winW = 60.0;
    float winH = 60.0;
    float gapX = 30.0;
    float gapY = 90.0;

    for (int row = 0; row < 3; row++) {
        float y = startY + row * gapY;

        // Left 3 windows
        for (int i = 0; i < 3; i++) {
            float x = startX + i * (winW + gapX);
            drawsquad(x, y, x, y + winH, x + winW, y + winH, x + winW, y);
        }

        // Center wide window
        float cx = startX + 3 * (winW + gapX);
        float cW = 190.0; // Wider center window
        drawsquad(cx, y, cx, y + winH, cx + cW, y + winH, cx + cW, y);

        // Right 3 windows (adjusted for center width)
        float offset = cx + cW + gapX;
        for (int i = 0; i < 3; i++) {
            float x = offset + i * (winW + gapX);
            drawsquad(x, y, x, y + winH, x + winW, y + winH, x + winW, y);
        }
    }


    //grass/bushes
    setColor255(41, 104, 64);
    drawCircle(63, 645.0, 100);
    drawCircle(1475, 645.0, 90);
    drawCircle(1345, 675.0, 70);



    drawTree(0, 0);
    drawTree(1300, 0);
    drawTree(900, 70);


    glColor3f(0.376f, 0.557f, 0.275f);
    drawsquad(0.0,660.0, 0.0,820.0, 1535.0,820.0, 1535.0,660.0);
    drawsquad(0.0,980.0, 0.0,1050.0, 975.0,1050.0, 1000.0,980.0);
    drawsquad(1290.0,980.0, 1270.0,1050.0, 1536.0,1050.0, 1536.0,980.0);




    //grass/bushes
    setColor255(41, 104, 64);
    drawCircle(0, 945.0, 100);
    drawCircle(100, 985.0, 90);
    drawCircle(220, 995.0, 70);
    drawCircle(320, 1050.0, 70);
    drawCircle(1500, 995.0, 70);
    drawCircle(1400, 1050.0, 70);



    drawGrass(350, 720, 7.0);
    drawGrass(880, 750, 2.0);
    drawGrass(650, 690, 4.0);
    drawGrass(850, 720, 7.0);
    drawGrass(980, 750, 2.0);
    drawGrass(1050, 690, 4.0);
    drawGrass(1450, 850, 2.0);
    drawGrass(1500, 730, 4.0);





    if (showDoor) {
        glPushMatrix();
        glTranslatef(1035.0f, 700.0f, 0.0f);
        glScalef(doorScale, doorScale, 1.0f);
        glTranslatef(-1035.0f, -700.0f, 0.0f);

        // Outer door
        setColor255(147, 192, 250);
        drawsquad(950.0, 525.0, 950.0, 880.0, 1120.0, 880.0, 1120.0, 525.0);

        // Middle oval
        setColor255(112, 146, 190);
        drawOval(1035.0, 690.0, 80.0, 180.0);

        // Inner oval
        setColor255(83, 108, 140);
        drawOval(1035.0, 690.0, 40.0, 140.0);

        glPopMatrix();

        drawDoor(-200, 0);
    }



    //seating area
    setColor255(152, 100, 44);
    drawsquad(40.0,620.0, 40.0,690.0, 310.0,690.0, 310.0,620.0);
    setColor255(82, 61, 19);
    drawsquad(60.0,690.0, 60.0,785.0, 90.0,785.0, 90.0,690.0);
    drawsquad(260.0,690.0, 260.0,785.0, 290.0,785.0, 290.0,690.0);
    setColor255(152, 100, 44);
    drawsquad(30.0,712.0, 30.0,736.0, 320.0,736.0, 320.0,712.0);
    setColor255(40, 105, 67);
    drawsquad(30.0,785.0, 30.0,795.0, 320.0,795.0, 320.0,785.0);




    drawGrass(350, 900, 3.0);
    drawGrass(880, 1020, 3.0);
    drawGrass(650, 1000, 3.0);



    // Target convergence point
    float targetX = 1035.0;
    float targetY = 690.0;

    // Person 0 (scaling toward target)
    if (person0Scale > 0.01f) {
        float offsetX = 800.0f + (targetX - 800.0f) * (1.0f - person1Scale);
        float offsetY = 800.0f + (targetY - 800.0f) * (1.0f - person1Scale);

        glPushMatrix();
        glTranslatef(offsetX, offsetY, 0);
        glScalef(person1Scale, person1Scale, 1.0f);
        drawHuman(0, 0, 0, 102, 204, 50, 50, 50);
        glPopMatrix();
    }

    // Person 1 (scaling toward target)
    if (person1Scale > 0.01f) {
        float offsetX = 500.0f + (targetX - 500.0f) * (1.0f - person1Scale);
        float offsetY = 800.0f + (targetY - 800.0f) * (1.0f - person1Scale);

        glPushMatrix();
        glTranslatef(offsetX, offsetY, 0);
        glScalef(person1Scale, person1Scale, 1.0f);
        drawHuman(0, 0, 0, 102, 204, 50, 50, 50);
        glPopMatrix();
    }

    // Person 2 (scaling toward target)
    if (person2Scale > 0.01f) {
        float offsetX = 650.0f + (targetX - 650.0f) * (1.0f - person2Scale);
        float offsetY = 800.0f + (targetY - 800.0f) * (1.0f - person2Scale);

        glPushMatrix();
        glTranslatef(offsetX, offsetY, 0);
        glScalef(person2Scale, person2Scale, 1.0f);
        drawHuman(0, 0, 200, 0, 0, 0, 0, 200);
        glPopMatrix();
    }

    // Person 3 (moving + scaling toward target)
    if (person3Scale > 0.01f) {
        float offsetX = humanX + (targetX - humanX) * (1.0f - person3Scale);
        float offsetY = 800.0f + (targetY - 800.0f) * (1.0f - person3Scale);

        glPushMatrix();
        glTranslatef(offsetX, offsetY, 0);
        glScalef(person3Scale, person3Scale, 1.0f);
        drawFlatHumanFacingRight(0, 0);
        glPopMatrix();
    }


    if (showBubbles && !bubblesDone) {
        SimpleBubble &b = fixedBubbles[currentBubbleIndex];
        drawSpeechBubble(b.x, b.y, b.message, b.len);
    }



    glutSwapBuffers();
}

void update(int value) {
    cloudPosition += 0.0004f;
    if (cloudPosition > 1.2f) {
        cloudPosition = -1.2f;
    }
    cloudPosition2 += 0.0008f;
    if (cloudPosition2 > 1.2f) {
        cloudPosition2 = -1.2f;
    }

    cloudPosition3 += 0.0005f;
    if (cloudPosition3 > 1.2f) {
        cloudPosition3 = -1.2f;
    }

    cloudPosition4 += 0.0008f;
    if (cloudPosition4 > 1.2f) {
        cloudPosition4 = -1.2f;
    }

    cloudPosition5 += 0.0005f;
    if (cloudPosition5 > 1.2f) {
        cloudPosition5 = -1.2f;
    }

    cloudPosition6 += 0.0007f;
    if (cloudPosition6 > 1.2f) {
        cloudPosition6 = -1.2f;
    }


    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}


void make_door_visible(int value) {
    showDoor = true;
    glutPostRedisplay();
}


void update_people_to_door(int value) {
    float targetX = 935.0f;
    float targetY = 690.0f;
    float moveSpeed = 1.0f;
    float scaleDecay = 0.005f;

    if (!scaleHum || !bubblesDone) {
        glutTimerFunc(16, update_people_to_door, 0);
        return;
    }

    if (scaleHum) {
        // Person 1
        float dx1 = targetX - person1X;
        float dy1 = targetY - person1Y;
        float dist1 = sqrt(dx1 * dx1 + dy1 * dy1);
        if (dist1 > 1.0f) {
            person1X += moveSpeed * dx1 / dist1;
            person1Y += moveSpeed * dy1 / dist1;
            if (person1Scale > 0.01f) person1Scale -= scaleDecay;
        }

        // Person 2
        float dx2 = targetX - person2X;
        float dy2 = targetY - person2Y;
        float dist2 = sqrt(dx2 * dx2 + dy2 * dy2);
        if (dist2 > 1.0f) {
            person2X += moveSpeed * dx2 / dist2;
            person2Y += moveSpeed * dy2 / dist2;
            if (person2Scale > 0.01f) person2Scale -= scaleDecay;
        }

        // Person 3
        float dx3 = targetX - person3X;
        float dy3 = targetY - person3Y;
        float dist3 = sqrt(dx3 * dx3 + dy3 * dy3);
        if (dist3 > 1.0f) {
            person3X += moveSpeed * dx3 / dist3;
            person3Y += moveSpeed * dy3 / dist3;
            if (person3Scale > 0.01f) person3Scale -= scaleDecay;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update_people_to_door, 0);

}


void showNextFixedBubble(int value) {
    if (currentBubbleIndex < fixedBubbleCount - 1) {
        currentBubbleIndex++;
        glutPostRedisplay();
        glutTimerFunc(3000, showNextFixedBubble, 0);
    } else {
        bubblesDone = true;
        scaleHum = true;
        update_people_to_door(0);
    }
}


void update_left_human(int value) {
    if (moveHuman) {
        humanX += 1.2f;
        if (humanX >= targetX) {
            humanX = targetX;
            moveHuman = false;
            showBubbles = true;
            currentBubbleIndex = 0;
            bubblesDone = false;
            glutTimerFunc(0, showNextFixedBubble, 0);
        }
        glutPostRedisplay();
        glutTimerFunc(16, update_left_human, 0);
    }
}

void updateDoorScale(int value) {
    doorScaleAngle += 0.05f;
    if (doorScaleAngle > 2 * 3.14159f) {
        doorScaleAngle = 0.0f;
    }

    doorScale = 1.0f + 0.05f * sin(doorScaleAngle);

    glutPostRedisplay();
    glutTimerFunc(16, updateDoorScale, 0);
}


void updateSun(int value) {
    sunAngle += 0.02f;
    if (sunAngle > 2 * 3.14159f) {
        sunAngle = 0.0f;
    }

    sunYOffset = 20.0f * sin(sunAngle);

    glutPostRedisplay();
    glutTimerFunc(16, updateSun, 0);
}







void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1535.0, 1035.0, 0.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Cloud, Sun , and Bird Animation");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    glutTimerFunc(0, update_left_human, 0);
    glutTimerFunc(22000, make_door_visible, 0);
    glutTimerFunc(8000, [](int){
        scaleHum = true;
        update_people_to_door(0);
    }, 0);
    glutTimerFunc(0, updateDoorScale, 0);
    glutTimerFunc(0, updateSun, 0);


    initGL();
    glutMainLoop();
    return 0;
}
