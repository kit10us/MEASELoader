// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <ase/AseDLL.h>
#include <ase/GeometryFactory.h>
#include <memory.h>
#include <me/render/IRenderer.h>
#include <me/game/Game.h>
#include <me/debug/ErrorLevel.h>

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
	auto debug = gameInstance->Debug();
	auto block{ debug->GetLogger()->CreateBlock( "MELoader \"MEASELoader\"" ) };

	const auto * texturePS = element->FindFirstElement( "textureps" );
	if ( ! texturePS )
	{
		debug->ReportError( debug::ErrorLevel::Failure, "Element \"textureps\" in \"" + element->GetDocument()->GetPath().ToString() + "\"." ); 
	}
	
	debug->Assert( texturePS != nullptr );

	std::string texturePSName = texturePS->GetAttribute< std::string >( "name" );
	unify::Path texturePSPath( texturePS->GetAttribute< std::string >( "source" ) );
	IPixelShader::ptr ps = gameInstance->GetManager< IPixelShader >()->Add( texturePSName, texturePSPath )();
	
	const auto * textureVS = element->FindFirstElement( "texturevs" );
	if (!texturePS)
	{
		debug->ReportError( debug::ErrorLevel::Failure, "Element \"texturevs\" in \"" + element->GetDocument()->GetPath().ToString() + "\"." );
	}

	std::string textureVSName = textureVS->GetAttribute< std::string >( "name" );
	unify::Path textureVSPath( textureVS->GetAttribute< std::string >( "source" ) );
	IVertexShader::ptr vs = gameInstance->GetManager< IVertexShader >()->Add( textureVSName, textureVSPath )();

	// Setup ASE factories.
	GeometryFactory * factory = new GeometryFactory( gameInstance );
	factory->SetVertexShader( vs );
	factory->SetPixelShader( ps );

	gameInstance->GetManager< Geometry >()->AddFactory( "ase", GeometryFactory::ptr( factory ) );


	return true;
}

