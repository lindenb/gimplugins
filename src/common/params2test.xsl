<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:import href="mod.plugin.xsl"/>
<xsl:output method="text"/>


<xsl:template match="/">
#include &lt;cstdio&gt;
#include &lt;cstdlib&gt;
#include &lt;cstring&gt;
#include &lt;string&gt;
#include &lt;libgimp/gimp.h&gt;
#include &lt;common/xcairo.hh&gt;

using namespace std;

class TestPlugin
	{
	public:
		<xsl:apply-templates select="//param" mode="field"/>
		TestPlugin();
		~TestPlugin();
		int main(int argc,char** argv);
	};

TestPlugin::TestPlugin()
	{
	}
TestPlugin::~TestPlugin()
	{
	}

int TestPlugin::main(int argc,char** argv)
	{
	return 0;
	}

int main(int argc,char** argv)
	{
	TestPlugin app;
	return app.main(argc,argv);
	}

</xsl:template>

</xsl:stylesheet>
