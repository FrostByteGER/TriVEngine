using System;
using System.Collections.Generic;
using System.Text;
using CppSharp;
using CppSharp.AST;
using CppSharp.Generators;
namespace TriVEngineCSharp.Source.Scripting.CSharp.BindingGenerator
{
    public class TestGenerator : ILibrary
    {
        public void Preprocess(Driver driver, ASTContext ctx)
        {

        }

        public void Postprocess(Driver driver, ASTContext ctx)
        {

        }

        public void Setup(Driver driver)
        {
            var options = driver.Options;
            options.GeneratorKind = GeneratorKind.CSharp;
            var module = options.AddModule("Test");
            module.IncludeDirs.Add(@"..\..\..\Engine\Core");
            module.Headers.Add("EngineClock.hpp");
            // module.LibraryDirs.Add();
        }

        public void SetupPasses(Driver driver)
        {

        }

        public static void Main(string[] args)
        {
            ConsoleDriver.Run(new TestGenerator());
        }
    }
}
