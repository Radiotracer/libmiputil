/**
 * @file version.h
 *
 * These routines provide a consistent method for querying the version
 * numbers for MIP libraries and applications.
 *
 * All library projects need to include the autogenerated file
 * buildinfo.c in ONE of their source files and your application
 * should include version.h (this file) in its main module, and
 * compile and link version.c.
 *
 * Note: Your application project does not need to explicitly include
 * buildinfo.h becuase it is implicitly included in this file.
 * 
 * These functions work in conjunction with a library or application
 * specific instance of the _buildinfo struct. The struct is globally defined
 * in buildinfo.h according to the name created from the PACKAGE_NAME
 * autoconf macro in the shell script buildinfo.sh called from the Makefile.
 *
 * You may then write a function to handle the --version switch and
 * get the version information from dependent libs by using the
 * functions declared in this file. The first argument to all of these
 * functions is a pointer to a specific instance of the _buildinfo
 * struct.
 *
 * eg. You may call:
 *
 *  char *summary = GetVersionSummary(&libirl_buildinfo);
 *  printf("%s\n", summary);
 *  free(summary);
 *
 * to get a version summary for a package named libirl.
 *
 * You may of course do this for all dependent libs that were linked
 * into your application and you should in fact do this.
 *
 * $Id: version.h 62 2006-01-30 16:08:31Z mjs $
 */

#ifndef VERSION_H
#define VERSION_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
	typedef struct _buildinfo {
		const char *name;
		const char *version;
		const char *date;
		const char *url;
		const char *dir;
		const char *user;
		const char *host;
		const char *revision;
		const char *kernel;
		const char *machine;
    const char *compiler;
    const char *compiler_version;
	} BuildInfo_t;
	
	/* These functions return a single field from the struct */

	const char* GetBuildName(struct _buildinfo*);
	const char* GetBuildVersion(struct _buildinfo*);
	const char* GetBuildDate(struct _buildinfo*);
	const char* GetBuildURL(struct _buildinfo*);
	const char* GetBuildDir(struct _buildinfo*);
	const char* GetBuildUser(struct _buildinfo*);
	const char* GetBuildHost(struct _buildinfo*);
	const char* GetBuildRevision(struct _buildinfo*);
	const char* GetBuildKernel(struct _buildinfo*);
	const char* GetBuildMachine(struct _buildinfo*);
	const char* GetBuildCompiler(struct _buildinfo*);
	const char* GetBuildCompilerVersion(struct _buildinfo*);

	/* These functions return a dynamically allocated aggregated string */

	const char* GetBuildSummary(struct _buildinfo *buildinfo);
	const char* GetBuildString(struct _buildinfo *buildinfo);

	/* Convenience functions */

	int CheckBuildVersion(struct _buildinfo*, int major, int minor, int patch);
	int CheckForVersionSwitch(int argc, char **argv);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* VERSION_H */