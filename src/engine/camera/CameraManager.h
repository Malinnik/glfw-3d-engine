#include <vector>
#include <memory>
#include "engine/camera/camera.h"

class CameraManager {
    public:

        static CameraManager& instance() {
            static CameraManager cameraManager;
            return cameraManager;
        }
        CameraManager(const CameraManager&) = delete;
        CameraManager& operator=(const CameraManager&) = delete;

        void registerCamera(Camera* camera);
        void unregisterCamera(Camera* camera);

        void pushCamera(Camera* camera);
        void popCamera();

        void setActiveCamera(Camera* camera);
        Camera* getActiveCamera() const;

        const std::vector<Camera*>& getAllCameras() const { return m_allCameras; }


    private:
        CameraManager() = default;
        ~CameraManager() = default;

        std::vector<Camera*> m_cameraStack;
        std::vector<Camera*> m_allCameras;
};