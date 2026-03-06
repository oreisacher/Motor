//
// Created by Orik on 20/09/2025.
//

#ifndef MOTOR_TEXTURE_H
#define MOTOR_TEXTURE_H

#include "Asset.h"

namespace Motor {
    class Texture : public Asset {
    public:
        enum Type {
            UNDEFINED   = 0,
            CUSTOM      = 1,
            DIFFUSE     = 2,
            METALLIC    = 3,
            ROUGHNESS   = 4,
            AO          = 5,
            NORMAL      = 6,
        };

        enum Filter {
            NEAREST,
            LINEAR
        };

        enum Wrap {
            REPEAT,
            MIRRORED_REPEAT,
            CLAMP_TO_EDGE
        };

        ~Texture() override;

        const int& getWidth() const;
        void setWidth(const int& width);

        const int& getHeight() const;
        void setHeight(const int& height);

        const int& getNrChannels() const;
        void setNrChannels(const int& nrChannels);

        const bool& isTransparent() const;
        void setTransparent(const bool& transparent);

        const Filter& getFilter() const;
        void setFilter(const Filter& filter);

        const Wrap& getWrap() const;
        void setWrap(const Wrap& wrap);

        const Type& getTextureType() const;
        void setTextureType(const Type& type);

        bool isColor() const;

        const unsigned char* getData() const;
        void setData(unsigned char* data);

        const char* getType() const;

        static Texture& GetEmpty();

    protected:
        int width = 0;
        int height = 0;
        int nrChannels = 0;
        bool transparent = false;

        Type type = UNDEFINED;
        Filter filter = NEAREST;
        Wrap wrap = REPEAT;

        unsigned char* data = nullptr;

    private:
        friend class TextureLoader;
    };
}

#endif //MOTOR_TEXTURE_H