#pragma once

#include "material.h"
#include "vector.h"
#include "object.h"
#include "light.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <array>

class Scene {
public:
    const std::vector<Object>& GetObjects() const {
        return objects_;
    }
    const std::vector<SphereObject>& GetSphereObjects() const {
        return spheres_;
    }
    const std::vector<Light>& GetLights() const {
        return lights_;
    }
    const std::unordered_map<std::string, Material>& GetMaterials() const {
        return materials_;
    }

    friend Scene ReadScene(const std::filesystem::path& path);

private:
    std::vector<Vector> vectors_;
    std::vector<Vector> normals_;
    std::vector<Object> objects_;
    std::vector<SphereObject> spheres_;
    std::vector<Light> lights_;
    std::unordered_map<std::string, Material> materials_;
};

std::unordered_map<std::string, Material> ReadMaterials(const std::filesystem::path& path) {
    std::unordered_map<std::string, Material> res;
    std::ifstream input(path);
    if (input.is_open()) {
        std::string line;
        std::string cur_name;
        Material cur;
        while (std::getline(input, line)) {
            std::string s = line;
            auto l =
                std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isspace(c); });
            auto r = std::find_if(s.rbegin(), s.rend(), [](unsigned char c) {
                         return !std::isspace(c);
                     }).base();
            if (l < r) {
                s = std::string(l, r);
            } else {
                s = std::string{};
            }
            if (s.rfind("newmtl ", 0) == 0) {
                if (!cur_name.empty()) {
                    res[cur_name] = cur;
                }
                cur_name = s.substr(7);
                auto nl = std::find_if(cur_name.begin(), cur_name.end(),
                                       [](unsigned char c) { return !std::isspace(c); });
                auto nr = std::find_if(cur_name.rbegin(), cur_name.rend(), [](unsigned char c) {
                              return !std::isspace(c);
                          }).base();
                if (nl < nr) {
                    cur_name = std::string(nl, nr);
                } else {
                    cur_name = std::string{};
                }
                cur = Material();
                cur.name = cur_name;
                cur.specular_exponent = 1.0;
                cur.refraction_index = 1.0;
                cur.albedo = Vector(1.0, 0.0, 0.0);
            } else if (s.rfind("Ka ", 0) == 0) {
                std::istringstream iss(s.substr(3));
                double r, g, b;
                iss >> r >> g >> b;
                cur.ambient_color = Vector(r, g, b);
            } else if (s.rfind("Kd ", 0) == 0) {
                std::istringstream iss(s.substr(3));
                double r, g, b;
                iss >> r >> g >> b;
                cur.diffuse_color = Vector(r, g, b);
            } else if (s.rfind("Ks ", 0) == 0) {
                std::istringstream iss(s.substr(3));
                double r, g, b;
                iss >> r >> g >> b;
                cur.specular_color = Vector(r, g, b);
            } else if (s.rfind("Ke ", 0) == 0) {
                std::istringstream iss(s.substr(3));
                double r, g, b;
                iss >> r >> g >> b;
                cur.intensity = Vector(r, g, b);
            } else if (s.rfind("Ns ", 0) == 0) {
                std::istringstream iss(s.substr(3));
                iss >> cur.specular_exponent;
            } else if (s.rfind("Ni ", 0) == 0) {
                std::istringstream iss(s.substr(3));
                iss >> cur.refraction_index;
            } else if (s.rfind("al ", 0) == 0) {
                std::istringstream iss(s.substr(3));
                double x, y, z;
                iss >> x >> y >> z;
                cur.albedo = Vector(x, y, z);
            }
        }
        if (!cur_name.empty()) {
            res[cur_name] = cur;
        }
        input.close();
    }
    return res;
}
Scene ReadScene(const std::filesystem::path& path) {
    Scene scene;
    std::ifstream input(path);
    if (!input.is_open()) {
        return scene;
    }
    std::vector<Vector> vectors;
    std::vector<Vector> normals;
    const Material* current_material = nullptr;
    std::vector<std::array<int, 3>> pending_normals;
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty() || line.starts_with("#")) {
            // skip
        } else if (line.rfind("vn ", 0) == 0 || line.rfind("vn\t", 0) == 0) {
            std::istringstream iss(line.substr(3));
            double x, y, z;
            iss >> x >> y >> z;
            normals.push_back(Vector(x, y, z));
        } else if (line.rfind("v ", 0) == 0 || line.rfind("v\t", 0) == 0) {
            std::istringstream iss(line.substr(1));
            double x, y, z;
            iss >> x >> y >> z;
            vectors.push_back(Vector(x, y, z));
        } else if (line.rfind("mtllib ", 0) == 0) {
            std::string filename = line.substr(7);
            auto start = std::find_if(filename.begin(), filename.end(),
                                      [](unsigned char c) { return !std::isspace(c); });
            auto end = std::find_if(filename.rbegin(), filename.rend(), [](unsigned char c) {
                           return !std::isspace(c);
                       }).base();
            filename = std::string(start, end);
            std::filesystem::path mpath = path.parent_path() / filename;
            scene.materials_ = ReadMaterials(mpath);
        } else if (line.rfind("usemtl ", 0) == 0) {
            std::string name = line.substr(7);
            auto start = std::find_if(name.begin(), name.end(),
                                      [](unsigned char c) { return !std::isspace(c); });
            auto end = std::find_if(name.rbegin(), name.rend(), [](unsigned char c) {
                           return !std::isspace(c);
                       }).base();
            name = std::string(start, end);
            if (scene.materials_.contains(name)) {
                current_material = &scene.materials_.at(name);
            }
        } else if (line.rfind("f ", 0) == 0 || line.rfind("f\t", 0) == 0) {
            std::istringstream iss(line.substr(1));
            std::vector<int> v_idx;
            std::vector<int> n_idx;
            std::string tok;
            while (iss >> tok) {
                int vi = 0;
                int ni = -1;
                const auto p1 = tok.find('/');
                if (p1 == std::string::npos) {
                    vi = std::stoi(tok);
                } else {
                    const std::string s0 = tok.substr(0, p1);
                    const auto p2 = tok.find('/', p1 + 1);
                    if (p2 == std::string::npos) {
                        vi = std::stoi(s0);
                    } else {
                        const std::string s2 = tok.substr(p2 + 1);
                        vi = std::stoi(s0);
                        if (!s2.empty()) {
                            ni = std::stoi(s2);
                        }
                    }
                }
                if (vi > 0) {
                    --vi;
                } else {
                    vi += static_cast<int>(vectors.size());
                }
                if (ni > 0) {
                    --ni;
                } else if (ni < 0) {
                    ni += static_cast<int>(normals.size());
                }
                v_idx.push_back(vi);
                n_idx.push_back(ni);
            }
            if (v_idx.size() >= 3) {
                for (size_t i = 1; i + 1 < v_idx.size(); ++i) {
                    int i0 = v_idx[0];
                    int i1 = v_idx[i];
                    int i2 = v_idx[i + 1];
                    if (i0 < 0 || i1 < 0 || i2 < 0 || static_cast<size_t>(i0) >= vectors.size() ||
                        static_cast<size_t>(i1) >= vectors.size() ||
                        static_cast<size_t>(i2) >= vectors.size()) {
                        continue;
                    }
                    Object object{current_material,
                                  Triangle(vectors[i0], vectors[i1], vectors[i2]),
                                  {nullptr, nullptr, nullptr}};
                    scene.objects_.push_back(object);
                    int ni0 = n_idx[0];
                    int ni1 = n_idx[i];
                    int ni2 = n_idx[i + 1];
                    pending_normals.push_back({ni0, ni1, ni2});
                }
            }
        } else if (line.rfind("S ", 0) == 0 || line.rfind("S\t", 0) == 0) {
            std::istringstream iss(line.substr(1));
            double x, y, z, r;
            iss >> x >> y >> z >> r;
            SphereObject sphere{current_material, Sphere(Vector(x, y, z), r)};
            scene.spheres_.push_back(sphere);
        } else if (line.rfind("P ", 0) == 0 || line.rfind("P\t", 0) == 0) {
            std::istringstream iss(line.substr(1));
            double x, y, z, r, g, b;
            iss >> x >> y >> z >> r >> g >> b;
            Light light;
            light.position = Vector(x, y, z);
            light.intensity = Vector(r, g, b);
            scene.lights_.push_back(light);
        }
    }
    scene.vectors_ = vectors;
    scene.normals_ = normals;
    for (size_t k = 0; k < scene.objects_.size() && k < pending_normals.size(); ++k) {
        auto& object = scene.objects_[k];
        const auto& pn = pending_normals[k];
        for (size_t j = 0; j < 3; ++j) {
            const int ni = pn[j];
            if (ni >= 0 && static_cast<size_t>(ni) < scene.normals_.size()) {
                object.normals[j] = &scene.normals_[ni];
            }
        }
    }
    input.close();
    return scene;
}
