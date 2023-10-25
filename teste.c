#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Variáveis globais para armazenar os dados do objeto
GLfloat *vertices = NULL;
int numVertices = 0;

// Função para carregar o arquivo .obj
void loadObjFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo .obj\n");
        exit(1);
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            numVertices++;
            vertices = (GLfloat *)realloc(vertices, numVertices * 3 * sizeof(GLfloat));
            sscanf(line, "v %f %f %f", &vertices[(numVertices - 1) * 3], &vertices[(numVertices - 1) * 3 + 1], &vertices[(numVertices - 1) * 3 + 2]);
        }
    }

    fclose(file);
}

// Função de renderização
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -5.0f);

    // Renderize seu objeto aqui
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < numVertices; i++) {
        glVertex3f(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
    }
    glEnd();

    glutSwapBuffers();
}

// Função principal
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Uso: %s <arquivo.obj>\n", argv[0]);
        return 1;
    }

    loadObjFile(argv[1]);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Objeto 3D");

    glutDisplayFunc(display);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    free(vertices); // Libera a memória alocada para os vértices

    return 0;
}
