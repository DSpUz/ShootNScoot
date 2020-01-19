#pragma once

#include "Ammo.h"
#include "EntityController.h"
#include "Rect.h"

namespace sns
{
	template<> class EntityController<Ammo>
	{
	public:
		static void Update( Ammo& model, float dt )noexcept;
		static void TakeDamage( Ammo& model, float amount )noexcept;

		static Ammo::Owner GetOwner( Ammo const& model )noexcept;
		static float Damage( Ammo const& model )noexcept;
		static RectF AABB( Ammo const& model )noexcept;
		static bool IsAlive( Ammo const& model )noexcept;
	};
}