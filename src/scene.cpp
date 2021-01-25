#include "scene.hpp"

/* Save the scene to disk. */
void Scene::Save() {

    m_dir = SAVE_DIR;
    std::filesystem::create_directories(m_dir + m_name);
    SaveWorld();
    SaveCamera();
    SaveConfig();
}

/* Save the world. */
void Scene::SaveWorld() {
    std::ofstream ofs;
    ofs.open(m_dir + m_name + "/world.cfg");
    ofs << m_world;
    ofs.close();
}

// TODO save camera, config
void Scene::SaveCamera() {

}

void Scene::SaveConfig() {

}

/* Load a scene from disk. */
void Scene::LoadScene() {
    m_dir = SCENE_DIR;
    m_world.Clear();
    m_config.Clear();
    strcpy(m_config.scene_name, m_name.c_str());
    LoadWorld();
}

void Scene::LoadSave() {
    assert(false);
}


/* Load world. */
void Scene::LoadWorld() {

    std::ifstream ifs;
    std::filesystem::path path {m_dir + m_name + "/world.cfg" };
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
