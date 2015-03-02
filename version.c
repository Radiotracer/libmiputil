/**
 * @file version.c
 *
 * The implementation of several functions used for querying the
 * version of MIP libraries and applications.
 * 
 * The functions defined here use the struct _buildinfo. The values
 * for this struct are generated at build time from macros which are defined in
 * config.h and by shell code and are written to buildinfo.h by the
 * Makefile. (The actual make targets and actions are coded in Makefile.am)
 * @see version.h for more details
 *
 * You should update the autoconf macro AC_INIT in configure.ac such
 * that the first argument is your package version string and the
 * second argument is the current release version and reconfigure the
 * package before building a release. There are more notes for this in
 * configure.ac
 *
 * @code $Id: version.c 62 2006-01-30 16:08:31Z mjs $ @endcode
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "version.h"

/**
 * @brief Get the package name.
 *
 * @return The package name as a string.
 */

const char* GetBuildName(struct _buildinfo *buildinfo)
{
	return buildinfo->name;
}

/**
 * @brief Get the package version.
 *
 * Format of the string is "MAJOR.MINOR.PATCH"
 *
 * @return The version number as a string.
 */

const char* GetBuildVersion(struct _buildinfo *buildinfo)
{
	return buildinfo->version;
}

/**
 * @brief Get the build date.
 *
 * @return The build date as a string.
 */

const char* GetBuildDate(struct _buildinfo *buildinfo)
{
	return buildinfo->date;
}

/**
 * @brief Get the build subversion URL
 *
 * @return The URL the repo was checked out from.
 */

const char* GetBuildURL(struct _buildinfo *buildinfo)
{
	return buildinfo->url;
}

/**
 * @brief Get the build directory.
 *
 * @return The build directory as a string.
 */

const char* GetBuildDir(struct _buildinfo *buildinfo)
{
	return buildinfo->dir;
}

/**
 * @brief Get the build user.
 *
 * @return The build user as a string.
 */

const char* GetBuildUser(struct _buildinfo *buildinfo)
{
	return buildinfo->user;
}

/**
 * @brief Get the build host.
 *
 * @return The build host as a string.
 */

const char* GetBuildHost(struct _buildinfo *buildinfo)
{
	return buildinfo->host;
}

/**
 * @brief Get the build revision.
 *
 * @return The subversion revision as a string.
 */

const char* GetBuildRevision(struct _buildinfo *buildinfo)
{
	return buildinfo->revision;
}

/**
 * @brief Get the build kernel.
 *
 * @return The build kernel as a string.
 */

const char* GetBuildKernel(struct _buildinfo *buildinfo)
{
	return buildinfo->kernel;
}

/**
 * @brief Get the build machine.
 *
 * @return The build machine as a string.
 */

const char* GetBuildMachine(struct _buildinfo *buildinfo)
{
	return buildinfo->machine;
}

/**
 * @brief Get compler name.
 *
 * @return The build compiler name as a string.
 */

const char* GetBuildCompiler(struct _buildinfo *buildinfo)
{
	return buildinfo->compiler;
}

/**
 * @brief Get compler version.
 *
 * @return The build compiler version as a string.
 */

const char* GetBuildCompilerVersion(struct _buildinfo *buildinfo)
{
	return buildinfo->compiler_version;
}

/**
 * @brief Check the internal version against the version represented
 * by the 3 args.
 *
 * The internal version number is composed from MAJOR.MINOR.PATCH
 *
 * @param major - The major version you want, true if @a major == MAJOR.
 * @param minor - The minor version you want, true if @a minor <= MINOR.
 * @param patch - The patch version you want, true if @a patch <= PATCH.
 *
 * @return int - 0 = false, 1 = true
 */

int CheckBuildVersion(struct _buildinfo *buildinfo, int major,
	int minor, int patch)
{
	int _major, _minor, _patch;
	sscanf(buildinfo->version, "%d.%d.%d", &_major, &_minor, &_patch);
	if (major == _major && minor <= _minor && patch <= _patch)
		return 1;
	else
		return 0;
}

/**
 * @brief Get a version summary.
 *
 * @Note: The caller needs to free the returned pointer.
 *
 * @return Several parameters formatted with linefeeds as a string.
 */

const char* GetBuildSummary(struct _buildinfo *buildinfo)
{
	char *summary = (char*) malloc(1024);

	int n = 0;

	n += sprintf(summary+n, "%s Version: %s\n", buildinfo->name,
		buildinfo->version);
	n += sprintf(summary+n, "Built by: %s@%s on %s\n",
		buildinfo->user, buildinfo->host, buildinfo->date);
	n += sprintf(summary+n, "Built from: %s\n", buildinfo->dir);
	n += sprintf(summary+n, "Built with: %s %s\n",
		buildinfo->compiler, buildinfo->compiler_version);
	n += sprintf(summary+n, "Subversion %s Revision: %s\n",
		buildinfo->url, buildinfo->revision);
	n += sprintf(summary+n, "Arch: %s Kernel: %s\n", buildinfo->machine,
		buildinfo->kernel);
	
	return summary;
}

/**
 * @brief Get a short version summary as a one line string
 *
 * @Note: The caller needs to free the returned pointer.
 *
 * @return Package name, version and revision.
 */

const char* GetBuildString(struct _buildinfo *buildinfo)
{
	char *summary = (char*) malloc(1024);

	sprintf(summary, "%s Version: %s Revision: %s",
		buildinfo->name, buildinfo->version, buildinfo->revision);
	
	return summary;
}

/**
 * @brief Check the arguments for --version
 *
 * @return int 1 if found, 0 otherwise.
 */

int CheckForVersionSwitch(int argc, char **argv)
{
	while (--argc)
		if (strstr(*++argv, "--version"))
			return 1;
	return 0;
}

#ifdef STAND_ALONE
int main(int argc, char **argv)
{

	char str[255];

	printf("%s\n", GetVersionSummary(&BUILD_INFO_NAME));

	printf("%s\n\n", GetVersionString(&BUILD_INFO_NAME));

	if (argc == 4)
	{
		if (CheckBuildVersion(&BUILD_INFO_NAME, atoi(argv[1]),atoi(argv[2]),atoi(argv[3])))
			printf("true");
		else
			printf("false");
	}
	else
		printf("Usage: %s major minor patch\n", argv[0]);
}
#endif
