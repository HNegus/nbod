#include "scene.hpp"

/* Save the scene to disk. */
void Scene::Save() {

    std::filesystem::create_directories(SCENE_DIR + m_name);
    SaveWorld();
    SaveCamera();
    SaveConfig();
}

/* Save the world. */
void Scene::SaveWorld() {
    std::ofstream ofs;
    ofs.open(SCENE_DIR + m_name + "/world.cfg");
    ofs << m_world;
    ofs.close();
}

// TODO save camera, config
void Scene::SaveCamera() {

}

void Scene::SaveConfig() {

}

/* Load a scene from disk. */
void Scene::Load() {
    m_world.Clear();
    m_config.Clear();
    LoadWorld();
}

/* Load world. */
void Scene::LoadWorld() {

    std::ifstream ifs;
    std::filesystem::path path {SCENE_DIR + m_name + "/world.cfg" };
    if (std::filesystem::exists(path)) {
        ifs.open(path);
        ifs >> m_world;
    }

    std::vector<Body *> bodies = m_world.GetBodies();
    int max_id = -1;
    int id;

    for (Body *body: bodies) {
        id = body->GetID();

        if (id > max_id) {
            max_id = (int) id;
        }
    }

    Body::SetIDCounter(++max_id);
    m_config.RegisterBodies(bodies);
}

// TODO load camera, config
void Scene::LoadCamera() {

}

void Scene::LoadConfig() {

}
