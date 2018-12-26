// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <ase/AseDLL.h>
#include <ase/GeometryFactory.h>
#include <memory.h>
#include <me/render/IRenderer.h>
#include <me/game/Game.h>

using namespace ase;
using namespace me;
using namespace render;

void Deleter( GeometryFactory * factory )
{
	delete factory;
}

extern "C" __declspec(dllexport) bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element );

__declspec(dllexport) bool MELoader( me::game::IGame * gameBase, const qxml::Element * element )
{
	using namespace me;

	auto gameInstance = dynamic_cast< game::Game * >( gameBase );

	const auto * texturePS = element->FindFirstElement( "textureps" );
	if ( ! texturePS )
	{
		gameInstance->Debug()->ReportError( me::ErrorLevel::Failure, "ASELoader", "Element \"textureps\" in \"" + element->GetDocument()->GetPath().ToString() + "\"." ); 
	}
	
	gameInstance->Debug()->Assert( texturePS != nullptr );

	std::string texturePSName = texturePS->GetAttribute< std::string >( "name" );
	unify::Path texturePSPath( texturePS->GetAttribute< std::string >( "source" ) );
	IPixelShader::ptr ps = gameInstance->GetManager< IPixelShader >()->Add( texturePSName, texturePSPath );
	
	const auto * textureVS = element->FindFirstElement( "texturevs" );
	if (!texturePS)
	{
		gameInstance->Debug()->ReportError( me::ErrorLevel::Failure, "ASELoader", "Element \"texturevs\" in \"" + element->GetDocument()->GetPath().ToString() + "\"." );
	}

	std::string textureVSName = textureVS->GetAttribute< std::string >( "name" );
	unify::Path textureVSPath( textureVS->GetAttribute< std::string >( "source" ) );
	IVertexShader::ptr vs = gameInstance->GetManager< IVertexShader >()->Add( textureVSName, textureVSPath );

	// Setup ASE factories.
	GeometryFactory * factory = new GeometryFactory( gameInstance );
	factory->SetVertexShader( vs );
	factory->SetPixelShader( ps );

	gameInstance->GetManager< Geometry >()->AddFactory( "ase", GeometryFactory::ptr( factory ) );


	return true;
}

