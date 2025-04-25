#pragma once

namespace Learning2DEngine
{
    namespace Physics
    {
        class IRigidbody
        {
        public:
            virtual ~IRigidbody() = default;

            virtual void ResetVelocityX() = 0;

            virtual void ResetVelocityY() = 0;
        };
    }
}
