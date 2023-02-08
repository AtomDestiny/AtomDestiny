#pragma once

class UWorld;

namespace AtomDestiny
{
    ///
    /// Represents world for unit testing.
    /// If you want to test some actions and code with UObject, AActor and so on,
    /// then use this FTestWorld
    /// 
    class FTestWorld
    {
    public:
        FTestWorld();
        ~FTestWorld();

        void Tick(float deltaSeconds);

        uint64 GetFrameCount() const { return m_frameCount; }

        UWorld* Get() const { return m_world; }
        UWorld* operator->() const { return Get(); }
        
    private:
        UWorld* m_world;
        uint64 m_frameCount = 1;
    };
    
} // namespace AtomDestiny
