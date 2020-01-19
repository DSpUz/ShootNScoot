#include "AngleMath.h"
#include "Bosses.h"
#include "BossController.h"
#include "ChiliMath.h"
#include "RectController.h"
#include "Vec2Controller.h"
#include "WeaponController.h"
#include "World.h"
#include <cassert>

namespace sns
{
	void EntityController<Boss>::Update( Boss& model, World& world, Vec2& hero_position, float dt ) noexcept
	{
		std::visit( [ & ]( auto& boss )
		{
			Update( boss, world, model, hero_position, dt );
		}, model.variant );
	}

	void EntityController<Boss>::TakeDamage( Boss& model, float amount ) noexcept
	{
		model.health -= amount;
	}

	float EntityController<Boss>::Damage( Boss const& model )noexcept
	{
		return std::visit( [ & ]( auto const& boss )
		{
			return std::decay_t<decltype( boss )>::damage;
		}, model.variant );
	}

	float EntityController<Boss>::Health( Boss const& model )noexcept
	{
		return model.health;
	}

	RectF EntityController<Boss>::AABB( Boss const& model )noexcept
	{
		return std::visit( [ & ]( auto const& boss )
		{
			return std::decay_t<decltype( boss )>::aabb + model.position;
		}, model.variant );
	}

	void EntityController<Boss>::Reset( Boss& model ) noexcept
	{
		model.position = { RectController::Center( world_rect ).x, world_rect.top };
		model.direction = { 0.f, -1.f };
		model.variant = Boss1{};

		std::visit( [ & ]( auto& boss ) 
		{ 
			model.health = boss.max_health; 
		}, model.variant );
	}

	void EntityController<Boss>::Update(  Boss1& model, World& world, Boss& parent, Vec2 const& hero_position, float dt )noexcept
	{
		switch( model.state )
		{
			case Boss1::State::Attack:
				WeaponController::Update( model.weapon, dt );

				if( WeaponController::CanFire( model.weapon ) )
				{
					const auto missile_dir =
						Vec2Controller::Normalize( hero_position - parent.position );

					WeaponController::Fire(
						model.weapon,
						parent.position,
						missile_dir,
						world,
						Ammo::Owner::Enemy
					);

					++model.missile_count;
				}

				if( model.missile_count >= model.max_missiles )
					model.state = Boss1::State::Regen;

				break;
			case Boss1::State::Regen:
				model.regen_timer -= dt;
				if( model.regen_timer <= 0.f )
				{
					model.missile_count = 0;
					model.regen_timer = model.regen_delay;
					model.state = Boss1::State::Attack;
				}
				break;
			case Boss1::State::Idle:
			{
				parent.position += ( parent.direction * ( Boss1::speed * dt ) );
				if( RectController::IsContainedBy( Boss1::aabb + parent.position, world_rect ) )
					model.state = Boss1::State::Attack;
				break;
			}
			default:
				assert( false );
		}
	}
	void EntityController<Boss>::Update(  Boss2& model, World& world, Boss& parent, Vec2 const& hero_position, float dt )noexcept
	{
		switch( model.state )
		{
			case Boss2::State::Idle:
			{
				parent.position += ( Vec2{ 0.f, 1.f } *( Boss2::speed * dt ) );
				if( RectController::IsContainedBy( Boss2::aabb + parent.position, world_rect ) )
				{
					model.state = Boss2::State::Signaling;
				}
				break;
			}
			case Boss2::State::Signaling:
			{
				model.signal_timer -= dt;
				if( model.signal_timer <= 0.f )
				{
					model.signal_timer = model.signal_delay;
					model.state = Boss2::State::Ramming;
				}
				else
				{
					model.waypoint = hero_position;

					parent.direction =
						Vec2Controller::Normalize( model.waypoint - parent.position );

					parent.angle = lerp(
						parent.angle, 
						std::atan2( parent.direction.y, parent.direction.x ),
						model.signal_timer / model.signal_delay 
					);
				}
				break;
			}
			case Boss2::State::Ramming:
			{
				parent.position += ( parent.direction * ( Boss2::speed * dt ) );
				if( Vec2Controller::LengthSq( model.waypoint - parent.position ) < 4.f )
				{
					model.state = Boss2::State::Recharging;
				}
				break;
			}
			case Boss2::State::Recharging:
			{
				model.cool_down_timer -= dt;
				if( model.cool_down_timer <= 0.f )
				{
					model.cool_down_timer = model.cool_down_delay;
					model.state = Boss2::State::Signaling;
				}
				break;
			}
			default:
				break;
		}
	}
	void EntityController<Boss>::Update(  Boss3& model, World& world, Boss& parent, Vec2& hero_position, float dt )noexcept
	{}
	void EntityController<Boss>::Update(  Boss4& model, World& world, Boss& parent, Vec2& hero_position, float dt )noexcept
	{}
	void EntityController<Boss>::Update(  Boss5& model, World& world, Boss& parent, Vec2& hero_position, float dt )noexcept
	{}
	void EntityController<Boss>::Update(  Boss6& model, World& world, Boss& parent, Vec2& hero_position, float dt )noexcept
	{}
	void EntityController<Boss>::Update(  Boss7& model, World& world, Boss& parent, Vec2& hero_position, float dt )noexcept
	{}
	void EntityController<Boss>::Update(  Boss8& model, World& world, Boss& parent, Vec2& hero_position, float dt )noexcept
	{}
	void EntityController<Boss>::Update(  Boss9& model, World& world, Boss& parent, Vec2& hero_position, float dt )noexcept
	{}
	void EntityController<Boss>::Update( Boss10& model, World& world, Boss& parent, Vec2& hero_position, float dt )noexcept
	{}
}