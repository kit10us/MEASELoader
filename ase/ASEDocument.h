// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <qxml/Document.h>

namespace ase
{
	class Document : public qxml::Document
	{
	public:
		unify::Result<> Load(unify::Path filePath);
	};
}