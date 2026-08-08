#include "CameraManager.h"


void CameraManager::registerCamera(Camera *camera)
{
    if (std::find(m_allCameras.begin(), m_allCameras.end(), camera) == m_allCameras.end()) {
        m_allCameras.push_back(camera);
    }
}

void CameraManager::unregisterCamera(Camera *camera)
{
    auto it = std::remove(m_allCameras.begin(), m_allCameras.end(), camera);
    m_allCameras.erase(it, m_allCameras.end());

    auto stackIt = std::remove(m_cameraStack.begin(), m_cameraStack.end(), camera);
    m_cameraStack.erase(stackIt, m_cameraStack.end());
}

void CameraManager::pushCamera(Camera *camera)
{
    registerCamera(camera);
    m_cameraStack.push_back(camera);
}

void CameraManager::popCamera()
{
    assert(!m_cameraStack.empty());
    m_cameraStack.pop_back();
}

void CameraManager::setActiveCamera(Camera *camera)
{
    if (!m_cameraStack.empty()) {
        m_cameraStack.back() = camera;
    } else {
        pushCamera(camera);
    }
}


Camera* CameraManager::getActiveCamera() const
{
    if (m_cameraStack.empty())
        return nullptr;
    else
        return m_cameraStack.back();
}
