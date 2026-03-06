

//
// Created by Orik on 20/09/2025.
//

#include <motor/assets/Asset.h>
#include <random>
#include <motor/utils/UUIDHelper.h>

using namespace Motor;

Asset::Asset() : name(""), path(""), id(UUIDHelper::generateUUID()) { }

const std::string & Asset::getId() const { return id; }

const std::string& Asset::getName() const { return name; }
void Asset::setName(const std::string &name) { this->name = name; }

const std::string& Asset::getPath() const { return path; }

const char * Asset::getType() const { return "Mesh"; }
