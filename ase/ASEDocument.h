// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <qxml/Document.h>

namespace ase
{
	class Document : public qxml::Document
	{
	public:
		void Load( unify::Path filePath );
	};
}