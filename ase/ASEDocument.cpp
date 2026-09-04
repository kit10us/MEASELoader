// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <ase/ASEDocument.h>
#include <unify/Path.h>
#include <unify/String.h>
#include <regex>

using namespace ase;

void ProcessASEElement( qxml::Element * pElement, std::string text );

unify::Result<> Document::Load( unify::Path filePath )
{
	// Destroy existing document
	Destroy();

	if( ! filePath.Exists() )
	{
		return unify::Failure(filePath.ToString() + ": XML File not found!" );
	}

	unify::FileStream stream;
	bool bInElement = false;
	bool bInComment = false;
	std::string sElementName;
	unsigned int uAttribute = 0;
	qxml::Attribute * pAttribute = 0;

	// Our initial parent is the root.
	qxml::Element * parent = AddElement( new qxml::Element( "Root", qxml::Element::NodeType::Element, nullptr, 0 ) );
	qxml::Element * previous = parent;

	stream.Open( unify::StreamAccessType::STREAMACCESS_READ, filePath );

	m_filePath = filePath;

	// Loop until we find an element, all else is an error (except cr, nl, spaces, tabs)
	std::string sData;
	char cQuotes = ' ';
	bool bInCData = false;
	char strData[ 256 ];
	size_t depth = 0;
	size_t line = 0;
	while( !stream.EndOfStream() )
	{
		stream.ReadPack( &strData, 256 );
					  
		size_t newDepth {};
		for( newDepth = 0; strData[newDepth] == '\t'; ++newDepth );

		if ( newDepth > depth )
		{
			parent = previous;
			depth = newDepth;
		}

		while( newDepth < depth )
		{
			parent = parent->GetParent();
			--depth;
		}

		sData = unify::String::TrimLeft( strData, '\t' );

		// Check that this is a valid element...
		if( sData[ 0 ] != '*' ) 
		{
			continue;
		}

		std::string name = unify::String::StringMinusLeft( unify::String::ListPart( sData, {' ', '\t' }, 0 ), 1 );
		qxml::Element * element = new qxml::Element( name, qxml::Element::NodeType::Element, nullptr, line );
		previous = element;
		AddElement( element );
		parent->TakeChild( element );

		// Ignore a curly brace'd element...
		if( unify::String::RightString( sData, 1 ) != "{" )
		{
			std::string text = std::regex_replace( sData, std::regex{ "^[*]" + name + "(\t|[ ])*" }, "" );
			
			// Allow modifying text...
			ProcessASEElement( element, text );
		}
		line++;
	}
	return {};
}

void ProcessASEElement( qxml::Element * pElement, std::string text )
{
	std::string tagName = pElement->GetName();
	if( unify::String::StringIs( tagName, "SCENE_AMBIENT_STATIC" ) )
	{
		text = unify::String::CleanWhitespace( text );
		pElement->AddAttribute( "r", unify::String::ListPart( text, {' ', '\t'}, 0 ) );
		pElement->AddAttribute( "g", unify::String::ListPart( text, {' ', '\t'}, 1 ) );
		pElement->AddAttribute( "b", unify::String::ListPart( text, {' ', '\t'}, 2 ) );
	}
	else if( unify::String::StringIs( tagName, "INHERIT_POS" ) )
	{
		text = unify::String::CleanWhitespace( text );
		pElement->AddAttribute( "x", unify::String::ListPart( text, {' ', '\t'}, 0 ) );
		pElement->AddAttribute( "y", unify::String::ListPart( text, {' ', '\t'}, 1 ) );
		pElement->AddAttribute( "z", unify::String::ListPart( text, {' ', '\t'}, 2 ) );
	}
	else if( unify::String::StringIs( tagName, "MESH_VERTEX" ) )
	{
		text = unify::String::CleanWhitespace( text );
		pElement->AddAttribute( "index", unify::String::ListPart( text, {' ', '\t'}, 0 ) );
		pElement->AddAttribute( "x", unify::String::ListPart( text, {' ', '\t'}, 1 ) );
		pElement->AddAttribute( "y", unify::String::ListPart( text, {' ', '\t'}, 2 ) );
		pElement->AddAttribute( "z", unify::String::ListPart( text, {' ', '\t'}, 3 ) );
	}
	else if( unify::String::StringIs( tagName, "MESH_VERTEXNORMAL" ) )
	{
		text = unify::String::CleanWhitespace( text );
		pElement->AddAttribute( "index", unify::String::ListPart( text, {' ', '\t'}, 0 ) );
		pElement->AddAttribute( "x", unify::String::ListPart( text, {' ', '\t'}, 1 ) );
		pElement->AddAttribute( "y", unify::String::ListPart( text, {' ', '\t'}, 2 ) );
		pElement->AddAttribute( "z", unify::String::ListPart( text, {' ', '\t'}, 3 ) );
	}
	else if( unify::String::StringIs( tagName, "MESH_FACENORMAL" ) )
	{
		text = unify::String::CleanWhitespace( text );
		pElement->AddAttribute( "index", unify::String::ListPart( text, {' ', '\t'}, 0 ) );
		pElement->AddAttribute( "x", unify::String::ListPart( text, {' ', '\t'}, 1 ) );
		pElement->AddAttribute( "y", unify::String::ListPart( text, {' ', '\t'}, 2 ) );
		pElement->AddAttribute( "z", unify::String::ListPart( text, {' ', '\t'}, 3 ) );
	}
	else if( unify::String::StringIs( tagName, "MESH_TVERT" ) )
	{
		text = unify::String::CleanWhitespace( text );
		pElement->AddAttribute( "index", unify::String::ListPart( text, {' ', '\t'}, 0 ) );
		pElement->AddAttribute( "u", unify::String::ListPart( text, {' ', '\t'}, 1 ) );
		pElement->AddAttribute( "v", unify::String::ListPart( text, {' ', '\t'}, 2 ) );
		pElement->AddAttribute( "w", unify::String::ListPart( text, {' ', '\t'}, 3 ) );
	}
	else if( unify::String::StringIs( tagName, "MESH_TFACE" ) )
	{
		text = unify::String::CleanWhitespace( text );
		pElement->AddAttribute( "index", unify::String::ListPart( text, {' ', '\t'}, 0 ) );
		pElement->AddAttribute( "A", unify::String::ListPart( text, {' ', '\t'}, 1 ) );
		pElement->AddAttribute( "B", unify::String::ListPart( text, {' ', '\t'}, 2 ) );
		pElement->AddAttribute( "C", unify::String::ListPart( text, {' ', '\t'}, 3 ) );
	}
	else if( unify::String::StringIs( tagName, "MESH_FACE" ) )
	{
		text = unify::String::CleanWhitespace( text );
		std::string sIndex = unify::String::StringMinusRight( unify::String::ListPart( text, {' ', '\t'}, 0 ), 1 );
		std::string sA = unify::String::ListPart( text, {' ', '\t'}, 2 );
		std::string sB = unify::String::ListPart( text, {' ', '\t'}, 4 );
		std::string sC = unify::String::ListPart( text, {' ', '\t'}, 6 );
		std::string sAB = unify::String::ListPart( text, {' ', '\t'}, 8 );
		std::string sBC = unify::String::ListPart( text, {' ', '\t'}, 10 );
		std::string sCA = unify::String::ListPart( text, {' ', '\t'}, 12 );
		std::string sMS = unify::String::ListPart( text, {' ', '\t'}, 14 );
		std::string sMM = unify::String::ListPart( text, {' ', '\t'}, 16 );
		pElement->AddAttribute( "index", sIndex );
		pElement->AddAttribute( "A", sA );
		pElement->AddAttribute( "B", sB );
		pElement->AddAttribute( "C", sC );
		pElement->AddAttribute( "AB", sAB );
		pElement->AddAttribute( "BC", sBC );
		pElement->AddAttribute( "CA", sCA );
		pElement->AddAttribute( "MESH_SMOOTHING", sMS );
		pElement->AddAttribute( "MESH_MTLID", sMM );
	}
	else if( unify::String::StringIs( tagName, "MATERIAL_AMBIENT" ) )
	{
		text = unify::String::CleanWhitespace( text );
		pElement->AddAttribute( "R", unify::String::ListPart( text, {' ', '\t'}, 0 ) );
		pElement->AddAttribute( "G", unify::String::ListPart( text, {' ', '\t'}, 1 ) );
		pElement->AddAttribute( "B", unify::String::ListPart( text, {' ', '\t'}, 2 ) );
	}
	else if( unify::String::StringIs( tagName, "MATERIAL_DIFFUSE" ) )
	{
		text = unify::String::CleanWhitespace( text );
		pElement->AddAttribute( "R", unify::String::ListPart( text, {' ', '\t'}, 0 ) );
		pElement->AddAttribute( "G", unify::String::ListPart( text, {' ', '\t'}, 1 ) );
		pElement->AddAttribute( "B", unify::String::ListPart( text, {' ', '\t'}, 2 ) );
	}
	else if( unify::String::StringIs( tagName, "MATERIAL_SPECULAR" ) )
	{
		text = unify::String::CleanWhitespace( text );
		pElement->AddAttribute( "R", unify::String::ListPart( text, {' ', '\t'}, 0 ) );
		pElement->AddAttribute( "G", unify::String::ListPart( text, {' ', '\t'}, 1 ) );
		pElement->AddAttribute( "B", unify::String::ListPart( text, {' ', '\t'}, 2 ) );
	}
	else if( unify::String::StringIs( tagName, "MATERIAL_NAME" ) || unify::String::StringIs( tagName, "MATERIAL_CLASS" ) || unify::String::StringIs( tagName, "MAP_NAME" ) || unify::String::StringIs( tagName, "BITMAP" ))
	{
		pElement->AddText( unify::String::Trim( text, '\"' ) );
	}
	else
	{
		pElement->AddText( text );
	}
}
