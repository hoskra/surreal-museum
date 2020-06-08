#pragma once

#include "../include/scene.h"
#include "../include/gameObject.h"

void Scene::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Scene::InitGameObjects()
{

	skybox->Init(); CHECK_GL_ERROR();
    shaderProgram.program = loader.CreateShader("data/shaders/color.vert", "data/shaders/color.frag");

    shaderProgram.timeLocation = glGetUniformLocation(shaderProgram.program, "time");
    shaderProgram.colorLocation = glGetAttribLocation(shaderProgram.program, "color");            CHECK_GL_ERROR();
    // get vertex attributes locations
    shaderProgram.posLocation = glGetAttribLocation(shaderProgram.program, "position");             CHECK_GL_ERROR();
    shaderProgram.normalLocation = glGetAttribLocation(shaderProgram.program, "normal");                CHECK_GL_ERROR();
    shaderProgram.texCoordLocation = glGetAttribLocation(shaderProgram.program, "texCoord");                CHECK_GL_ERROR();
    // get uniforms locations
    shaderProgram.PVMmatrixLocation = glGetUniformLocation(shaderProgram.program, "PVMmatrix");             CHECK_GL_ERROR();
    shaderProgram.VmatrixLocation = glGetUniformLocation(shaderProgram.program, "Vmatrix");             CHECK_GL_ERROR();
    shaderProgram.MmatrixLocation = glGetUniformLocation(shaderProgram.program, "Mmatrix");             CHECK_GL_ERROR();
    shaderProgram.normalMatrixLocation = glGetUniformLocation(shaderProgram.program, "normalMatrix");               CHECK_GL_ERROR();
    // material
    shaderProgram.ambientLocation = glGetUniformLocation(shaderProgram.program, "material.ambient");                CHECK_GL_ERROR();
    shaderProgram.diffuseLocation = glGetUniformLocation(shaderProgram.program, "material.diffuse");                CHECK_GL_ERROR();
    shaderProgram.specularLocation = glGetUniformLocation(shaderProgram.program, "material.specular");              CHECK_GL_ERROR();
    shaderProgram.shininessLocation = glGetUniformLocation(shaderProgram.program, "material.shininess");                CHECK_GL_ERROR();
    // texture
    shaderProgram.texSamplerLocation = glGetUniformLocation(shaderProgram.program, "texSampler");               CHECK_GL_ERROR();
    shaderProgram.useTextureLocation = glGetUniformLocation(shaderProgram.program, "material.useTexture");              CHECK_GL_ERROR();
    // lights
    shaderProgram.pointLight1positionLocation = glGetUniformLocation(shaderProgram.program, "pointLight1position");             CHECK_GL_ERROR();
    shaderProgram.spotLight1positionLocation = glGetUniformLocation(shaderProgram.program, "spotLight1position");             CHECK_GL_ERROR();
    shaderProgram.reflectorDirectionLocation = glGetUniformLocation(shaderProgram.program, "reflectorDirection");               CHECK_GL_ERROR();
    
    
    shaderProgram.textureTransformLocation = glGetUniformLocation(shaderProgram.program, "textureTransform");               CHECK_GL_ERROR();
    shaderProgram.optionLocation = glGetUniformLocation(shaderProgram.program, "option");               CHECK_GL_ERROR();

                                                                                                                                     
    if (!loader.loadSingleMesh("data/models/shoppingCart_0.obj", shaderProgram, &cart_0)) std::cout << "Loading cart failed" << std::endl;
    if (!loader.loadSingleMesh("data/models/shoppingCart_1.obj", shaderProgram, &cart_1)) std::cout << "Loading cart failed" << std::endl;
    if (!loader.loadSingleMesh("data/models/shoppingCart_2.obj", shaderProgram, &cart_2)) std::cout << "Loading cart failed" << std::endl;
    if (!loader.loadSingleMesh("data/models/grass.obj", shaderProgram, &grass)) std::cout << "Loading grass failed" << std::endl;
    if (!loader.loadSingleMesh("data/models/bush.obj", shaderProgram, &bush_0)) std::cout << "Loading bush failed" << std::endl;
    if (!loader.loadSingleMesh("data/models/varhany.obj", shaderProgram, &varhany)) std::cout << "Loading table failed" << std::endl;
    if (!loader.loadSingleMesh("data/models/stone_0.obj", shaderProgram, &stone_0)) std::cout << "Loading stone0 failed" << std::endl;
    if (!loader.loadSingleMesh("data/models/stone_1.obj", shaderProgram, &stone_1)) std::cout << "Loading stone1 failed" << std::endl;
    if (!loader.loadSingleMesh("data/models/stone_2.obj", shaderProgram, &stone_2)) std::cout << "Loading stone2 failed" << std::endl;

    if (!loader.loadSingleMesh("data/models/painting2.obj", shaderProgram, &painting_s)) std::cout << "Loading paintingS failed" << std::endl;
    if (!loader.loadSingleMesh("data/models/painting2.obj", shaderProgram, &painting_h)) std::cout << "Loading paintingH failed" << std::endl;
    if (!loader.loadSingleMesh("data/models/painting2.obj", shaderProgram, &painting_v)) std::cout << "Loading paintingV failed" << std::endl;

    if (!loader.loadSingleMesh("data/models/umbrella.obj", shaderProgram, &umbrella)) std::cout << "Loading umbrella failed" << std::endl;
    if (!loader.loadSingleMesh("data/models/autopsy_0.obj", shaderProgram, &autopsy_0)) std::cout << "Loading autopsy table0 failed" << std::endl;
    if (!loader.loadSingleMesh("data/models/autopsy_1.obj", shaderProgram, &autopsy_1)) std::cout << "Loading autopsy table1 failed" << std::endl;
    if (!loader.loadSingleMesh("data/models/sewing.obj", shaderProgram, &sewing)) std::cout << "Loading sewing machine failed" << std::endl;

    if (!loader.loadSingleMesh("data/models/petunia.obj", shaderProgram, &petunia)) std::cout << "Loading petunia failed" << std::endl;
    if (!loader.loadSingleMesh("data/models/whale.obj", shaderProgram, &whale)) std::cout << "Loading whale failed" << std::endl;

    if (!loader.loadSingleMesh("data/models/amanita_stem.obj", shaderProgram, &amanita_stem)) std::cout << "Loading amanita stem failed" << std::endl;
    if (!loader.loadSingleMesh("data/models/amanita_hat.obj", shaderProgram, &amanita_hat)) std::cout << "Loading amanita hat failed" << std::endl;

    if (!loader.loadSingleMesh("data/models/light.obj"           , shaderProgram, &pointLight1)) std::cout << "Loading amanita hat failed"  << std::endl;
    if (!loader.loadSingleMesh("data/models/light.obj"           , shaderProgram, &spotLight1)) std::cout << "Loading amanita hat failed"  << std::endl;
   

    painting_textures_h[0] = pgr::createTexture("data/models/paintings/h0.jpg");
    painting_textures_h[1] = pgr::createTexture("data/models/paintings/h1.jpg");
    painting_textures_h[2] = pgr::createTexture("data/models/paintings/h2.jpg");
    painting_textures_h[3] = pgr::createTexture("data/models/paintings/h3.jpg");
    painting_textures_h[4] = pgr::createTexture("data/models/paintings/h4.jpg");
    painting_textures_h[5] = pgr::createTexture("data/models/paintings/h5.jpg");
    painting_textures_h[6] = pgr::createTexture("data/models/paintings/h6.jpg");
    painting_textures_h[7] = pgr::createTexture("data/models/paintings/h7.jpg");
    painting_textures_h[8] = pgr::createTexture("data/models/paintings/h8.jpg");
    
    painting_textures_s[0] = pgr::createTexture("data/models/paintings/0.jpg");
    painting_textures_s[1] = pgr::createTexture("data/models/paintings/1.jpg");
    painting_textures_s[2] = pgr::createTexture("data/models/paintings/2.jpg");
    painting_textures_s[3] = pgr::createTexture("data/models/paintings/3.jpg");
    painting_textures_s[4] = pgr::createTexture("data/models/paintings/4.jpg");
    painting_textures_s[5] = pgr::createTexture("data/models/paintings/5.jpg");
    painting_textures_s[6] = pgr::createTexture("data/models/paintings/6.jpg");
    painting_textures_s[7] = pgr::createTexture("data/models/paintings/7.jpg");
    painting_textures_s[8] = pgr::createTexture("data/models/paintings/8.jpg");
    
    painting_textures_v[0] = pgr::createTexture("data/models/paintings/v0.jpg");
    painting_textures_v[1] = pgr::createTexture("data/models/paintings/v1.jpg");
    painting_textures_v[2] = pgr::createTexture("data/models/paintings/v2.jpg");
    painting_textures_v[3] = pgr::createTexture("data/models/paintings/v3.jpg");
    painting_textures_v[4] = pgr::createTexture("data/models/paintings/v4.jpg");
    painting_textures_v[5] = pgr::createTexture("data/models/paintings/v5.jpg");
    painting_textures_v[6] = pgr::createTexture("data/models/paintings/v6.jpg");
    painting_textures_v[7] = pgr::createTexture("data/models/paintings/v7.jpg");
    painting_textures_v[8] = pgr::createTexture("data/models/paintings/v8.jpg");


    SetObjects();
    SetMaterials();
}


void Scene::SetObjects(void) {
    // --------- gallery
    painting_s->m_position = glm::vec3(-14.45, 6.55, 33.4498);
    painting_s->m_size = 8.0f;
    painting_s->m_angle = 180.0f;
    painting_s->m_direction = glm::vec3(0.0f, 1.0f, 0.0f);

    painting_v->m_position = glm::vec3(-11.45, 5.2, 18.2);
    painting_v->m_size = 5.0f;
    painting_v->m_angle = 180.0f;
    painting_v->m_direction = glm::vec3(0.0f, 1.0f, 0.0f);

    painting_h->m_position = glm::vec3(14.55, 6, 25.1998);
    painting_h->m_size = 7.0f;
    painting_h->m_angle = -90.0f;
    painting_h->m_direction = glm::vec3(1.0f, 0.0f, 0.0f);

    // --------- environment
    grass->m_position = glm::vec3(0.0f, -1.0f, 0.0f);
    grass->m_size = 25.0f;

    bush_1 = new GameObject(*bush_0);
    bush_0->m_position = glm::vec3(9.15f, 0.0999998f, 9.80002f);
    bush_0->m_size = 1.8f;
    bush_1->m_position = glm::vec3(-9.35f, 0.16067f, -13.1f);
    bush_1->m_size = 1.8f;
    bush.push_back(bush_0);
    bush.push_back(bush_1);

    varhany->m_position = glm::vec3(7.05f, 1.3f, -9.90002f);
    varhany->m_size = 2.4f;
    varhany->m_angle = 110.0f;
    varhany->m_direction = glm::vec3(0.0f, -1.0f, 0.0f);

    stone_0->m_position = glm::vec3(-15.45f, 7.85002f, -0.15f);
    stone_0->m_size = 10.0f;
    stone_1->m_position = glm::vec3(0.3f, 8.00002f, -0.25f);
    stone_1->m_size = 10.0f;
    stone_1->m_angle = 180.0f;
    stone_1->m_direction = glm::vec3(0.0f, -1.0f, 0.0f);
    stone_2->m_position = glm::vec3(16.25f, 5.05001f, 0.45f);
    stone_2->m_size = 10.0f;
    stone.push_back(stone_0);
    stone.push_back(stone_1);
    stone.push_back(stone_2);

    // --------- cart
    cart_0->m_position = glm::vec3(0, 1.9, 0);
    cart_0->m_size = 3.0f;
    cart_1->m_position = glm::vec3(0.000569172, 4.55, -2.2);
    cart_1->m_size = 1.65f;
    cart_2->m_position = glm::vec3(0, -0.5, -0.1);
    cart_2->m_size = 3.0f;

    cart.push_back(cart_0);
    cart.push_back(cart_1);
    cart.push_back(cart_2);

    // --------- breton
    sewing->m_position = glm::vec3(-10.7494, 6.0, 30.7498);
    sewing->m_size = 1.0f;
    sewing->m_angle = 90.0f;
    sewing->m_direction = glm::vec3(0.0f, 2.0f, 0.0f);

    umbrella->m_position = glm::vec3(-10.65, 6.25, 33.2998);
    umbrella->m_size = 2.0f;
    umbrella->m_angle = 90.0f;
    umbrella->m_direction = glm::vec3(0.0f, 1.0f, 0.0f);

    autopsy_0->m_position = glm::vec3(-10.75, -0.15, 31.7998);
    autopsy_0->m_size = 10.0f;
    autopsy_0->m_angle = 90.0f;
    autopsy_0->m_direction = glm::vec3(0.0f, 1.0f, 0.0f);

    autopsy_1->m_position = glm::vec3(-10.5994, 0.71, 28.4998);
    autopsy_1->m_size = 4.29998f;
    autopsy_1->m_angle = 90.0f;
    autopsy_1->m_direction = glm::vec3(0.0f, 1.0f, 0.0f);

    autopsy.push_back(autopsy_0);
    autopsy.push_back(autopsy_1);

    // --------- hitchhiker
    whale->m_position = glm::vec3(10.2506, 15.4, 28.8998);
    whale->m_size = 3.85;
    whale->m_angle = 270.0f;
    whale->m_direction = glm::vec3(0.1f, 1.2f, 0.0f);

    petunia->m_position = glm::vec3(11.9501, 18.45, 22.2499);
    petunia->m_size = 1.0f;
    petunia->m_angle = 20.0f;
    petunia->m_direction = glm::vec3(0.0f, 0.3f, 0.4f);

    // --------- amanita
    amanita_stem->m_position = glm::vec3(-13.1499, -0.3f, 13.8499);
    amanita_stem->m_size = 4.0f;
    amanita_hat->m_position = glm::vec3(-13.1499, 0.1f, 13.8499);
    amanita_hat->m_size = 4.4f;

    // --------- light
    pointLight1->m_size = 0.05f;
    pointLight1->m_position = glm::vec3(-13.1499, 2.3f, 13.8499);

    spotLight1->m_size = 0.03f;
    spotLight1->m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    //spotLight1->m_position = glm::vec3(-13.45, 6.55, 33.4498);
}

void Scene::SetMaterials(void) {
    grass->m_ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    grass->m_diffuse = glm::vec3(0.25f, 0.25f, 0.1f);
    grass->m_specular = glm::vec3(0.0499998f, 0.0f, 0.0f);
    grass->m_shininess = 1.0f;

    umbrella->m_ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    umbrella->m_diffuse = glm::vec3(0.95f, 0.65f, 0.75f);
    umbrella->m_specular = glm::vec3(0.4f, 0.4f, 0.4f);
    umbrella->m_shininess = 1.0f;

    cart_0->m_ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    cart_0->m_diffuse = glm::vec3(0.8f, 0.85f, 0.8f);
    cart_0->m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
    cart_0->m_shininess = 1.0f;

    cart_1->m_ambient = glm::vec3(1.0f, 0.35f, 0.1f);
    cart_1->m_diffuse = glm::vec3(0.9f, 0.45f, 0.1f);
    cart_1->m_specular = glm::vec3(0.0f, 0.0f, 0.0f);
    cart_1->m_shininess = 1.0f;

    cart_2->m_ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    cart_2->m_diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    cart_2->m_specular = glm::vec3(0.25f, 0.25f, 0.25f);
    cart_2->m_shininess = 1.0f;

    amanita_hat->m_ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    amanita_hat->m_diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    amanita_hat->m_specular = glm::vec3(0.0f, 0.0f, 0.0f);
    amanita_hat->m_shininess = 1.0f;

    whale->m_ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    whale->m_diffuse = glm::vec3(0.15f, 0.25f, 0.3f);
    whale->m_specular = glm::vec3(1.0f, 0.9f, 1.0f);
    whale->m_shininess = 1.0f;
    
    autopsy_0->m_specular = glm::vec3(0.0f, 0.0f, 0.0f);
    autopsy_1->m_specular = glm::vec3(0.0f, 0.0f, 0.0f);
    sewing->m_specular = glm::vec3(0.0f, 0.0f, 0.0f);
    stone_0->m_specular = glm::vec3(0.0f, 0.0f, 0.0f);
    stone_1->m_specular = glm::vec3(0.0f, 0.0f, 0.0f);
    stone_2->m_specular = glm::vec3(0.0f, 0.0f, 0.0f);

    painting_h->m_ambient = glm::vec3(0.9f, 1.0f, 1.0f);
    painting_h->m_diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    painting_h->m_specular = glm::vec3(0.0f, 0.0f, 0.0f);
    painting_s->m_ambient = glm::vec3(0.9f, 1.0f, 1.0f);
    painting_s->m_diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    painting_s->m_specular = glm::vec3(0.0f, 0.0f, 0.0f);
    painting_v->m_ambient = glm::vec3(0.9f, 1.0f, 1.0f);
    painting_v->m_diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    painting_v->m_specular = glm::vec3(0.0f, 0.0f, 0.0f);

    pointLight1->m_ambient = glm::vec3(0.0f, 1.f, 1.0f);
    pointLight1->m_diffuse = glm::vec3(0.0f, 0.f, 0.8f);
    pointLight1->m_specular = glm::vec3(0.0f, 0.5f, 0.5f);
}

void Scene::DrawRepeatingGroup(std::vector<GameObject*> group, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, unsigned int count, float start, unsigned int spacing) {
    for (unsigned int i = 0; i <= count; i++) {
        DrawStaticGroup(group, viewMatrix, projectionMatrix, -1.8f, 0.0f, start + spacing * i);
        CHECK_GL_ERROR();
    }
}

void Scene::DrawRepeatingObject(GameObject* object, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, unsigned int count, float start, unsigned int spacing) {
    for (unsigned int i = 0; i <= count; i++) {
        DrawStatic(object, viewMatrix, projectionMatrix, -1.8f, 0.0f, start + spacing * i);
        CHECK_GL_ERROR();
    }
}

void Scene::DrawLight(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x, float y, float z) {
    GameObject* object = pointLight1;

    glUseProgram(shaderProgram.program);  CHECK_GL_ERROR();

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(
        object->m_position.x + x,
        object->m_position.y + y,
        object->m_position.z + z
    ));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(object->m_angle), object->m_direction);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(object->m_size, object->m_size, object->m_size));

    glUniform3fv(shaderProgram.pointLight1positionLocation, 1, glm::value_ptr(object->m_position));

    SetTransformUniforms(modelMatrix, viewMatrix, projectionMatrix); CHECK_GL_ERROR();
    SetMaterialUniforms(object);

    glUniform1i(shaderProgram.texSamplerLocation, 0);CHECK_GL_ERROR();

    glBindVertexArray(object->vertexArrayObject);

    glBindTexture(GL_TEXTURE_2D, object->m_texture);
    glDrawElements(GL_TRIANGLES, object->numTriangles * 3, GL_UNSIGNED_INT, 0); CHECK_GL_ERROR();

    glBindVertexArray(0); CHECK_GL_ERROR();



    glUseProgram(0);
    glUseProgram(shaderProgram.program);  CHECK_GL_ERROR();



    object = spotLight1;

    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(
        object->m_position.x + x,
        object->m_position.y + y,
        object->m_position.z + z
    ));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(object->m_angle), object->m_direction);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(object->m_size, object->m_size, object->m_size));

    glUniform3fv(shaderProgram.spotLight1positionLocation, 1, glm::value_ptr(object->m_position));


    SetTransformUniforms(modelMatrix, viewMatrix, projectionMatrix); CHECK_GL_ERROR();
    SetMaterialUniforms(object);

    glUniform1i(shaderProgram.texSamplerLocation, 0);CHECK_GL_ERROR();

    glBindVertexArray(object->vertexArrayObject);

    glBindTexture(GL_TEXTURE_2D, object->m_texture);
    glDrawElements(GL_TRIANGLES, object->numTriangles * 3, GL_UNSIGNED_INT, 0); CHECK_GL_ERROR();

    glBindVertexArray(0); CHECK_GL_ERROR();
    glUseProgram(0);
}

void Scene::DrawStatic(GameObject* object, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x, float y, float z) {
    glUseProgram(shaderProgram.program);  CHECK_GL_ERROR();

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(
        object->m_position.x + x,
        object->m_position.y + y,
        object->m_position.z + z
    ));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(object->m_angle), object->m_direction);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(object->m_size, object->m_size, object->m_size));

    SetTransformUniforms(modelMatrix, viewMatrix, projectionMatrix); CHECK_GL_ERROR();
    SetMaterialUniforms(object);

    glUniform1i(shaderProgram.texSamplerLocation, 0);CHECK_GL_ERROR();

    glBindVertexArray(object->vertexArrayObject);

    glBindTexture(GL_TEXTURE_2D, object->m_texture);
    float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glDrawElements(GL_TRIANGLES, object->numTriangles * 3, GL_UNSIGNED_INT, 0); CHECK_GL_ERROR();

    glBindVertexArray(0); CHECK_GL_ERROR();
    glUseProgram(0);
}

void Scene::DrawPainting(GameObject* object, GLuint * texture, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x, float y, float z) {
    glUseProgram(shaderProgram.program);  CHECK_GL_ERROR();

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(
        object->m_position.x + x,
        object->m_position.y + y,
        object->m_position.z + z
    ));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(object->m_angle), object->m_direction);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(object->m_size, object->m_size, object->m_size));

    SetTransformUniforms(modelMatrix, viewMatrix, projectionMatrix); CHECK_GL_ERROR();
    SetMaterialUniforms(object);

    glUniform1i(shaderProgram.texSamplerLocation, 0);CHECK_GL_ERROR();

    glBindVertexArray(object->vertexArrayObject);
    float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    glBindTexture(GL_TEXTURE_2D, texture[painting]);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glDrawElements(GL_TRIANGLES, object->numTriangles * 3, GL_UNSIGNED_INT, 0); CHECK_GL_ERROR();

    glBindVertexArray(0); CHECK_GL_ERROR();
    glUseProgram(0);

}

void Scene::DrawCart(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    glStencilFunc(GL_ALWAYS, 1, -1);

    glUseProgram(shaderProgram.program);  CHECK_GL_ERROR();

    for (auto object : cart) {
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f) , glm::vec3(
                object->m_position.x,
                object->m_position.y,
                object->m_position.z + clock
        ));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(object->m_angle), object->m_direction);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(object->m_size, object->m_size, object->m_size));

        SetTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);
        SetMaterialUniforms(object);

        glBindVertexArray(object->vertexArrayObject);
        glDrawElements(GL_TRIANGLES, object->numTriangles * 3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
    glUseProgram(0);
    return;
};

void Scene::DrawAmanita(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x, float y, float z) {
    glStencilFunc(GL_ALWAYS, 12, -1);

    glUseProgram(shaderProgram.program);  CHECK_GL_ERROR();

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f) , glm::vec3(
        amanita_hat->m_position.x + x,
        amanita_hat->m_position.y + y,
        amanita_hat->m_position.z + z
    ));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(amanita_hat->m_angle), amanita_hat->m_direction);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(amanita_hat->m_size, amanita_hat->m_size, amanita_hat->m_size));

    SetTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

    glm::mat3 textureTransform = glm::mat3(
        alpha, 0, 0,
        0, alpha, 0,
        0, 0, 1
    );

    textureTransform *= glm::mat3(
        cos(time), sin(time), 0,
        -sin(time), cos(time), 0,
        0, 0, 1
    );

    glUniform3fv(shaderProgram.diffuseLocation, 1, glm::value_ptr(amanita_hat->m_diffuse));
    glUniform3fv(shaderProgram.ambientLocation, 1, glm::value_ptr(amanita_hat->m_ambient));
    glUniform3fv(shaderProgram.specularLocation, 1, glm::value_ptr(amanita_hat->m_specular));
    glUniform1f(shaderProgram.shininessLocation, amanita_hat->m_shininess);
    CHECK_GL_ERROR();
    glUniform1i(shaderProgram.optionLocation, 1);
    glUniformMatrix3fv(shaderProgram.textureTransformLocation, 1, GL_FALSE, glm::value_ptr(textureTransform));   CHECK_GL_ERROR();

    glUniform1i(shaderProgram.useTextureLocation, 1);
    glUniform1i(shaderProgram.texSamplerLocation, 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, amanita_hat->m_texture);

    glBindVertexArray(amanita_hat->vertexArrayObject);
    glDrawElements(GL_TRIANGLES, amanita_hat->numTriangles * 3, GL_UNSIGNED_INT, 0);
    CHECK_GL_ERROR();
    glBindVertexArray(0);

    glUseProgram(0);
    glUseProgram(shaderProgram.program);  CHECK_GL_ERROR();

    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(
        amanita_stem->m_position.x + x,
        amanita_stem->m_position.y + y,
        amanita_stem->m_position.z + z
    ));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(amanita_stem->m_angle), amanita_stem->m_direction);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(amanita_stem->m_size, amanita_stem->m_size, amanita_stem->m_size));

    SetTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

    glUniform3fv(shaderProgram.diffuseLocation, 1, glm::value_ptr(amanita_stem->m_diffuse));
    glUniform3fv(shaderProgram.ambientLocation, 1, glm::value_ptr(amanita_stem->m_ambient));
    glUniform3fv(shaderProgram.specularLocation, 1, glm::value_ptr(amanita_stem->m_specular));
    glUniform1f(shaderProgram.shininessLocation, amanita_stem->m_shininess);
    CHECK_GL_ERROR();
    glUniform1i(shaderProgram.optionLocation, 0);

    glUniform1i(shaderProgram.useTextureLocation, 1);
    glUniform1i(shaderProgram.texSamplerLocation, 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, amanita_stem->m_texture);

    glBindVertexArray(amanita_stem->vertexArrayObject);
    glDrawElements(GL_TRIANGLES, amanita_stem->numTriangles * 3, GL_UNSIGNED_INT, 0);
    CHECK_GL_ERROR();
    glBindVertexArray(0);
    glUseProgram(0);

    return;
};

void Scene::DrawUmbrella(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x , float y , float z, char iteration ) {
    GameObject* object = umbrella;

    glUseProgram(shaderProgram.program);  CHECK_GL_ERROR();

    int signum = change ? -1 : 1;

    float mx = object->m_position.x + x + signum*sin((time - (double)(iteration*2)) * 0.7) * 3;
    float my = object->m_position.y + y + signum*cos((time - (double)(iteration*2)) * 0.5) * 4;
    float mz = object->m_position.z + z + signum*cos((time - (double)(iteration*2)) * 0.3) * 3;

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(mx, my, mz));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(alpha * 90), object->m_direction);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(object->m_size / iteration, object->m_size / iteration, object->m_size / iteration));

    SetTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);
    SetMaterialUniforms(object);

    glBindVertexArray(object->vertexArrayObject);
    glDrawElements(GL_TRIANGLES, object->numTriangles * 3, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
    return;
};

void Scene::DrawSewing(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x, float y, float z ) {
    GameObject* object = sewing;

    glUseProgram(shaderProgram.program);  CHECK_GL_ERROR();
    
    int signum = change ? -1 : 1;

    float mx = object->m_position.x + x + signum*sin(time * 0.7) * 3;
    float my = object->m_position.y + y + signum*cos(time * 0.5) * 4;
    float mz = object->m_position.z + z + signum*cos(time * 0.3) * 3;
    
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(mx, my, mz));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(object->m_angle * alpha), object->m_direction);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(object->m_size, object->m_size, object->m_size));

    SetTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);
    SetMaterialUniforms(object);

    glBindVertexArray(object->vertexArrayObject);
    glDrawElements(GL_TRIANGLES, object->numTriangles * 3, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
    return;
};

void Scene::DrawFalling(GameObject* object, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x, float y, float z ) {

    glUseProgram(shaderProgram.program);  CHECK_GL_ERROR();
    
    float my = object->m_position.y + y - clock / 2;
    if (my <= 0 && clock <= 70) {
        my = 0;
    }
    if (clock >= 70) {
        my = object->m_position.y + y;
    }

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(
        object->m_position.x + x,
        my,
        object->m_position.z + z
    ));

    modelMatrix = glm::rotate(modelMatrix, glm::radians(object->m_angle), object->m_direction);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(object->m_size, object->m_size, object->m_size));

    SetTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);
    SetMaterialUniforms(object);

    glBindVertexArray(object->vertexArrayObject);
    glDrawElements(GL_TRIANGLES, object->numTriangles * 3, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
    return;
};

void Scene::DrawStaticGroup(std::vector<GameObject*> group, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float x, float y, float z, float scale) {
    glUseProgram(shaderProgram.program);  CHECK_GL_ERROR();

    for (auto object : group) {
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(
            object->m_position.x + x,
            object->m_position.y + y,
            object->m_position.z + z
        ));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(object->m_angle), object->m_direction);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(object->m_size * scale, object->m_size * scale, object->m_size * scale));

        SetTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);
        SetMaterialUniforms(object);

        glBindVertexArray(object->vertexArrayObject);
        glDrawElements(GL_TRIANGLES, object->numTriangles * 3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

    glUseProgram(0);
    return;
};

void Scene::DrawGallery(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, unsigned int count, float start, unsigned int spacing) {
    for (unsigned int i = 0; i <= count; i++) {
        glStencilFunc(GL_ALWAYS, 4, -1);
        DrawPainting(painting_s, painting_textures_s, viewMatrix, projectionMatrix, 0.0f, 0.0f, start + spacing * i);
        CHECK_GL_ERROR();

        glStencilFunc(GL_ALWAYS, 5, -1);
        DrawPainting(painting_h, painting_textures_h, viewMatrix, projectionMatrix, 0.0f, 0.0f, start + spacing * i);
        CHECK_GL_ERROR();

        glStencilFunc(GL_ALWAYS, 6, -1);
        DrawPainting(painting_v, painting_textures_v, viewMatrix, projectionMatrix, 0.0f, 0.0f, start + spacing * i);
        CHECK_GL_ERROR();
    }
};

void Scene::DrawHitchhiker(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, unsigned int count, float start, unsigned int spacing) {
    for (unsigned int i = 0; i <= count; i++) {
        glStencilFunc(GL_ALWAYS, 7, -1);
        DrawFalling(petunia, viewMatrix, projectionMatrix, 0.0f, 0.0f, start + spacing * i);
        CHECK_GL_ERROR();

        glStencilFunc(GL_ALWAYS, 8, -1);
        DrawFalling(whale, viewMatrix, projectionMatrix, 0.0f, 0.0f, start + spacing * i);
        CHECK_GL_ERROR();
    }
};

void Scene::DrawBreton(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, unsigned int count, float start, unsigned int spacing) {
    for (unsigned int i = 0; i <= count; i++) {
        glStencilFunc(GL_ALWAYS, 9, -1);
        DrawSewing(viewMatrix, projectionMatrix, 0, 0, 0);							    
        CHECK_GL_ERROR();
        DrawSewing(viewMatrix, projectionMatrix, 0, 0, start + spacing * i);						    
        CHECK_GL_ERROR();

        glStencilFunc(GL_ALWAYS, 10, -1);
        DrawStaticGroup(autopsy, viewMatrix, projectionMatrix, 0, 0, start + spacing * i, 0.5f);	    
        CHECK_GL_ERROR();

        glDisable(GL_CULL_FACE);

        glStencilFunc(GL_ALWAYS, 11, -1);
        DrawUmbrella(viewMatrix, projectionMatrix, 0, 0, start + spacing * i);			        
        CHECK_GL_ERROR();
        DrawUmbrella(viewMatrix, projectionMatrix, 0, 0, start + spacing * i, 2);	            
        CHECK_GL_ERROR();
        DrawUmbrella(viewMatrix, projectionMatrix, 0, 0, start + spacing * i, 3);	            
        CHECK_GL_ERROR();
    }
};

void Scene::SetTransformUniforms(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    glm::mat4 PVM = projectionMatrix * viewMatrix * modelMatrix;
    glUniformMatrix4fv(shaderProgram.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));

    glUniformMatrix4fv(shaderProgram.VmatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shaderProgram.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // just take 3x3 rotation part of the modelMatrix
    // we presume the last row contains 0,0,0,1
    const glm::mat4 modelRotationMatrix = glm::mat4(
        modelMatrix[0],
        modelMatrix[1],
        modelMatrix[2],
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
    //glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelRotationMatrix));

    //or an alternative single-line method: 
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(glm::mat4(glm::mat3(modelRotationMatrix))));

    glUniformMatrix4fv(shaderProgram.normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));  // correct matrix for non-rigid transform
}

void Scene::SetMaterialUniforms(GameObject* object) {
    glUniform3fv(shaderProgram.diffuseLocation, 1, glm::value_ptr(object->m_diffuse));  CHECK_GL_ERROR();
    glUniform3fv(shaderProgram.ambientLocation, 1, glm::value_ptr(object->m_ambient)); CHECK_GL_ERROR();
    glUniform3fv(shaderProgram.specularLocation, 1, glm::value_ptr(object->m_specular)); CHECK_GL_ERROR();
    glUniform1f(shaderProgram.shininessLocation, object->m_shininess);

    glUniform1i(shaderProgram.optionLocation, 0); CHECK_GL_ERROR();

    if (object->m_texture) {
        glUniform1i(shaderProgram.useTextureLocation, 1);  // do texture sampling
        glUniform1i(shaderProgram.texSamplerLocation, 0);  // texturing unit 0 -> samplerID   [for the GPU linker]
        glActiveTexture(GL_TEXTURE0 + 0);                  // texturing unit 0 -> to be bound [for OpenGL BindTexture]
        glBindTexture(GL_TEXTURE_2D, object->m_texture);
    }
    else {
        glUniform1i(shaderProgram.useTextureLocation, 0);  // do not sample the texture
    }
    CHECK_GL_ERROR();
}

void Scene::ResetSize() {
    painting_s->m_size = 8.0f;
    painting_v->m_size = 5.0f;
    painting_h->m_size = 7.0f;

    pgr::deleteProgramAndShaders(shaderProgram.program);
    shaderProgram.program = loader.CreateShader("data/shaders/color.vert", "data/shaders/color.frag");

    shaderProgram.timeLocation = glGetUniformLocation(shaderProgram.program, "time");
    shaderProgram.colorLocation = glGetAttribLocation(shaderProgram.program, "color");            CHECK_GL_ERROR();
    // get vertex attributes locations
    shaderProgram.posLocation = glGetAttribLocation(shaderProgram.program, "position");             CHECK_GL_ERROR();
    shaderProgram.normalLocation = glGetAttribLocation(shaderProgram.program, "normal");                CHECK_GL_ERROR();
    shaderProgram.texCoordLocation = glGetAttribLocation(shaderProgram.program, "texCoord");                CHECK_GL_ERROR();
    // get uniforms locations
    shaderProgram.PVMmatrixLocation = glGetUniformLocation(shaderProgram.program, "PVMmatrix");             CHECK_GL_ERROR();
    shaderProgram.VmatrixLocation = glGetUniformLocation(shaderProgram.program, "Vmatrix");             CHECK_GL_ERROR();
    shaderProgram.MmatrixLocation = glGetUniformLocation(shaderProgram.program, "Mmatrix");             CHECK_GL_ERROR();
    shaderProgram.normalMatrixLocation = glGetUniformLocation(shaderProgram.program, "normalMatrix");               CHECK_GL_ERROR();
    // material
    shaderProgram.ambientLocation = glGetUniformLocation(shaderProgram.program, "material.ambient");                CHECK_GL_ERROR();
    shaderProgram.diffuseLocation = glGetUniformLocation(shaderProgram.program, "material.diffuse");                CHECK_GL_ERROR();
    shaderProgram.specularLocation = glGetUniformLocation(shaderProgram.program, "material.specular");              CHECK_GL_ERROR();
    shaderProgram.shininessLocation = glGetUniformLocation(shaderProgram.program, "material.shininess");                CHECK_GL_ERROR();
    // texture
    shaderProgram.texSamplerLocation = glGetUniformLocation(shaderProgram.program, "texSampler");               CHECK_GL_ERROR();
    shaderProgram.useTextureLocation = glGetUniformLocation(shaderProgram.program, "material.useTexture");              CHECK_GL_ERROR();
    // lights
    shaderProgram.pointLight1positionLocation = glGetUniformLocation(shaderProgram.program, "pointLight1position");             CHECK_GL_ERROR();
    shaderProgram.spotLight1positionLocation = glGetUniformLocation(shaderProgram.program, "spotLight1position");             CHECK_GL_ERROR();
    shaderProgram.reflectorDirectionLocation = glGetUniformLocation(shaderProgram.program, "reflectorDirection");             CHECK_GL_ERROR();


    shaderProgram.textureTransformLocation = glGetUniformLocation(shaderProgram.program, "textureTransform");               CHECK_GL_ERROR();
    shaderProgram.optionLocation = glGetUniformLocation(shaderProgram.program, "option");               CHECK_GL_ERROR();

}