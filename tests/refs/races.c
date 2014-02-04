#include "clar_libgit2.h"

#include "repository.h"
#include "git2/reflog.h"
#include "reflog.h"
#include "ref_helpers.h"

static const char *commit_id = "099fabac3a9ea935598528c27f866e34089c2eff";
static const char *refname = "refs/heads/master";
static const char *other_commit_id = "a65fedf39aefe402d3bb6e24df4d4f5fe4547750";

static git_repository *g_repo;

void test_refs_races__initialize(void)
{
   g_repo = cl_git_sandbox_init("testrepo");
}

void test_refs_races__cleanup(void)
{
   cl_git_sandbox_cleanup();
}

void test_refs_races__create_matching(void)
{
	int error;
	git_reference *ref, *ref2, *ref3;
	git_oid id, other_id;

	git_oid_fromstr(&id, commit_id);
	git_oid_fromstr(&other_id, other_commit_id);

	cl_git_fail_with(GIT_EMODIFIED, git_reference_create_matching(&ref, g_repo, refname, &other_id, 1, NULL, NULL, &other_id));

	cl_git_pass(git_reference_lookup(&ref, g_repo, refname));
	cl_git_pass(git_reference_create_matching(&ref2, g_repo, refname, &other_id, 1, NULL, NULL, &id));
	cl_git_fail_with(GIT_EMODIFIED, git_reference_set_target(&ref3, ref, &other_id, NULL, NULL));

	git_reference_free(ref);
	git_reference_free(ref2);
	git_reference_free(ref3);
}
