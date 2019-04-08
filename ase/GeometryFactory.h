// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <me/render/Mesh.h>
#include <rm/ResourceManager.h>
#include <me/game/Game.h>

namespace ase
{
	class GeometryFactory : public rm::ISourceFactory< me::render::Geometry >
	{
	public:
		typedef std::shared_ptr< rm::ISourceFactory< me::render::Geometry> > ptr;

		GeometryFactory( me::game::Game * gameInstance );

		void SetVertexShader( me::render::IVertexShader::ptr vertexShader );
		void SetPixelShader( me::render::IPixelShader::ptr pixelShader );

		me::render::Geometry::ptr Produce( unify::Path source, unify::Parameters parameters ) override;
		me::render::Geometry::ptr Produce( unify::Parameters parameters ) override;

	private:
	me::game::Game *m_game;
		// TODO: This will likely need to have multiple sets based on material needs - TBD.
		me::render::IVertexShader::ptr m_vertexShader;
		me::render::IPixelShader::ptr m_pixelShader;
	};
}