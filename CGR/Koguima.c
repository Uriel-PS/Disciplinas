//cd C:/Users/13875039955/Desktop/cgr
//gcc koguima.c -o koguima.exe -lopengl32 -lglu32 -lfreeglut
#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define WINDOW_W 1000
#define WINDOW_H 700

#define PI 3.14159265358979323846f

static float walkTime = 0.0f;
static float waveTime = 0.0f;
static float bodyBob = 0.0f;

static float leftArmAngle = 0.0f;
static float rightArmAngle = 0.0f;
static float rightArmPose = 0.0f;
static float rightArmSidePose = 0.0f;
static float leftLegAngle = 0.0f;
static float rightLegAngle = 0.0f;
static float headAngle = 0.0f;

static float waveUpperArm = 0.0f;
static float waveUpperArmSide = 0.0f;
static float waveForearm = 0.0f;
static float waveForearmSide = 0.0f;
static float waveHand = 0.0f;
static float waveHandSide = 0.0f;

static bool paused = false;
static bool waving = false;

static void drawCube(float w, float h, float d) {
    glPushMatrix();
    glScalef(w, h, d);
    glutSolidCube(1.0f);
    glPopMatrix();
}

static void drawGround(void) {
    glColor3f(0.20f, 0.24f, 0.30f);
    glBegin(GL_QUADS);
    glVertex3f(-12.0f, -3.0f, -6.0f);
    glVertex3f(12.0f, -3.0f, -6.0f);
    glVertex3f(12.0f, -3.0f, 6.0f);
    glVertex3f(-12.0f, -3.0f, 6.0f);
    glEnd();

    glColor3f(0.10f, 0.12f, 0.16f);
    for (int i = -12; i <= 12; i++) {
        glBegin(GL_LINES);
        glVertex3f((float)i, -2.99f, -6.0f);
        glVertex3f((float)i, -2.99f, 6.0f);
        glVertex3f(-12.0f, -2.99f, (float)i * 0.5f);
        glVertex3f(12.0f, -2.99f, (float)i * 0.5f);
        glEnd();
    }
}

static void drawArm(float upperAngle, float upperSideAngle, float forearmAngle, float forearmSideAngle, float handAngle, float handSideAngle) {
    glRotatef(upperSideAngle, 0.0f, 0.0f, 1.0f);
    glRotatef(upperAngle, 1.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, -0.75f, 0.0f);
    glColor3f(0.35f, 0.75f, 0.95f);
    drawCube(0.38f, 1.5f, 0.38f);
    glPopMatrix();

    glTranslatef(0.0f, -1.5f, 0.0f);
    glRotatef(forearmSideAngle, 0.0f, 0.0f, 1.0f);
    glRotatef(forearmAngle, 1.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, -0.60f, 0.0f);
    glColor3f(0.25f, 0.55f, 0.85f);
    drawCube(0.30f, 1.2f, 0.30f);
    glPopMatrix();

    glTranslatef(0.0f, -1.20f, 0.0f);
    glRotatef(handSideAngle, 0.0f, 0.0f, 1.0f);
    glRotatef(handAngle, 1.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, -0.18f, 0.0f);
    glColor3f(0.85f, 0.85f, 0.88f);
    drawCube(0.24f, 0.36f, 0.24f);
    glPopMatrix();
}

static void drawLeg(float upperAngle, float lowerAngle) {
    glRotatef(upperAngle, 1.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, -0.95f, 0.0f);
    glColor3f(0.85f, 0.45f, 0.22f);
    drawCube(0.42f, 1.9f, 0.42f);
    glPopMatrix();

    glTranslatef(0.0f, -1.9f, 0.0f);
    glRotatef(lowerAngle, 1.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, -0.75f, 0.0f);
    glColor3f(0.95f, 0.55f, 0.30f);
    drawCube(0.36f, 1.5f, 0.36f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -1.55f, 0.22f);
    glColor3f(0.20f, 0.20f, 0.24f);
    drawCube(0.46f, 0.22f, 0.85f);
    glPopMatrix();
}

static void drawRobot(void) {
    glPushMatrix();
    glTranslatef(0.0f, -0.2f + bodyBob, 0.0f);

    glPushMatrix();
    glColor3f(0.75f, 0.78f, 0.82f);
    drawCube(1.8f, 2.2f, 0.95f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 1.65f, 0.0f);
    glRotatef(headAngle, 0.0f, 1.0f, 0.0f);
    glColor3f(0.90f, 0.90f, 0.95f);
    drawCube(1.05f, 1.05f, 1.0f);

    glPushMatrix();
    glTranslatef(-0.22f, 0.08f, 0.52f);
    glColor3f(0.15f, 0.15f, 0.18f);
    glutSolidSphere(0.08f, 18, 18);
    glTranslatef(0.44f, 0.0f, 0.0f);
    glutSolidSphere(0.08f, 18, 18);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.1f, 0.85f, 0.0f);
    drawArm(leftArmAngle, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.1f, 0.85f, 0.0f);
    drawArm(rightArmPose, rightArmSidePose, 18.0f + waveForearm, waveForearmSide, waveHand, waveHandSide);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.48f, -1.15f, 0.0f);
    drawLeg(leftLegAngle, fabsf(leftLegAngle) * 0.45f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.48f, -1.15f, 0.0f);
    drawLeg(rightLegAngle, fabsf(rightLegAngle) * 0.45f);
    glPopMatrix();

    glPopMatrix();
}

static void updateAnimation(void) {
    if (paused) {
        return;
    }

    walkTime += 0.07f;

    leftLegAngle = sinf(walkTime) * 30.0f;
    rightLegAngle = sinf(walkTime + PI) * 30.0f;

    leftArmAngle = sinf(walkTime + PI) * 24.0f;
    rightArmAngle = sinf(walkTime) * 24.0f;
    rightArmPose = rightArmAngle;
    rightArmSidePose = 0.0f;

    bodyBob = fabsf(sinf(walkTime * 2.0f)) * 0.12f;
    headAngle = sinf(walkTime * 0.5f) * 6.0f;

    if (waving) {
        waveTime += 0.07f;
        float phase = waveTime;

        if (phase < 1.80f) {
            float t = phase / 1.80f;
            waveUpperArm = rightArmAngle + (-128.0f - rightArmAngle) * t;
            waveUpperArmSide = 24.0f * t;
            waveForearm = -18.0f * t;
            waveForearmSide = 0.0f;
            waveHand = 0.0f;
            waveHandSide = 0.0f;
        } else if (phase < 7.50f) {
            float localTime = phase - 1.80f;
            waveUpperArm = -128.0f;
            waveUpperArmSide = 22.0f + sinf(localTime * 2.2f) * 7.0f;
            waveForearm = -24.0f;
            waveForearmSide = sinf(localTime * 2.9f) * 5.0f;
            waveHand = sinf(localTime * 2.9f) * 8.0f;
            waveHandSide = sinf(localTime * 2.9f) * 18.0f;
        } else if (phase < 9.60f) {
            float t = (phase - 7.50f) / 2.10f;
            float endWaveTime = 5.70f;
            float endWaveUpperSide = 22.0f + sinf(endWaveTime * 2.2f) * 7.0f;
            float endWaveSideSin = sinf(endWaveTime * 2.9f);
            waveUpperArm = -128.0f + (rightArmAngle + 128.0f) * t;
            waveUpperArmSide = endWaveUpperSide * (1.0f - t);
            waveForearm = -24.0f * (1.0f - t);
            waveForearmSide = (endWaveSideSin * 5.0f) * (1.0f - t);
            waveHand = (endWaveSideSin * 8.0f) * (1.0f - t);
            waveHandSide = (endWaveSideSin * 18.0f) * (1.0f - t);
        } else {
            waveUpperArm = 0.0f;
            waveUpperArmSide = 0.0f;
            waveForearm = 0.0f;
            waveForearmSide = 0.0f;
            waveHand = 0.0f;
            waveHandSide = 0.0f;
            waving = false;
            waveTime = 0.0f;
        }

        /* Durante o aceno, o ombro direito assume uma pose dedicada
           em vez de somar duas animacoes que brigam entre si. */
        rightArmPose = waveUpperArm;
        rightArmSidePose = waveUpperArmSide;
    } else {
        waveTime = 0.0f;
        waveUpperArm = 0.0f;
        waveUpperArmSide = 0.0f;
        waveForearm = 0.0f;
        waveForearmSide = 0.0f;
        waveHand = 0.0f;
        waveHandSide = 0.0f;
    }
}

static void renderText(float x, float y, const char *text) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, WINDOW_W, 0.0, WINDOW_H);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glColor3f(0.95f, 0.95f, 0.95f);
    glRasterPos2f(x, y);
    for (const char *c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 2.5, 10.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    drawGround();
    drawRobot();

    renderText(15.0f, WINDOW_H - 20.0f, "Ciclico: caminhada | Nao ciclico: aceno");
    renderText(15.0f, WINDOW_H - 40.0f, "[P] pausa  [A] executar aceno  [ESC] sair");

    glutSwapBuffers();
}

static void reshape(int w, int h) {
    if (h == 0) {
        h = 1;
    }

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w / (double)h, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

static void timer(int value) {
    (void)value;
    updateAnimation();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

static void keyboard(unsigned char key, int x, int y) {
    (void)x;
    (void)y;

    switch (key) {
        case 'p':
        case 'P':
            paused = !paused;
            break;
        case 'a':
        case 'A':
            waving = true;
            waveTime = 0.0f;
            break;
        case 27:
            exit(0);
            break;
    }
}

static void initGL(void) {
    GLfloat lightPos[] = {4.0f, 8.0f, 6.0f, 1.0f};
    GLfloat ambient[] = {0.22f, 0.22f, 0.24f, 1.0f};
    GLfloat diffuse[] = {0.95f, 0.95f, 0.95f, 1.0f};

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glClearColor(0.06f, 0.07f, 0.10f, 1.0f);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_W, WINDOW_H);
    glutCreateWindow("Koguima");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
