coSetCppProjectDefaults("render")
postbuildcommands
{
    "{COPY} %{cfg.basedir}/shaders/* %{cfg.targetdir}/shaders"
}