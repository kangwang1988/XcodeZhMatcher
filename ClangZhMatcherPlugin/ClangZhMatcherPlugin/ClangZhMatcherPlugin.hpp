/*
 *  ClangZhMatcherPlugin.hpp
 *  ClangZhMatcherePlugin
 *
 *  Created by KyleWong on 26/10/2016.
 *  Copyright © 2016 KyleWong. All rights reserved.
 *
 */

#ifndef ClangZhMatcherPlugin_
#define ClangZhMatcherPlugin_

#include<iostream>
#include<sstream>
#include<typeinfo>

#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Lex/Lexer.h"
#include "ZhMatcherUtil.hpp"
/**
 * @discussion Enviroment:clang-3.9.1(release 39)
 * In terminal, use "/opt/llvm/llvm_build/bin/clang ../test.m -Xclang -load -Xclang /opt/llvm/clangplugin/libClangZhMatcherePlugin.dylib -Xclang -plugin -Xclang ClangZhMatcherPlugin".
 * In Xcode, use "-Xclang -load -Xclang /opt/llvm/clangplugin/libClangZhMatcherPlugin.dylib -Xclang -add-plugin -Xclang ClangZhMatcherPlugin",notice the -add-plugin instead of -plugin.
 *****XcodeHacking
 * sudo mv HackedClang.xcplugin `xcode-select -print-path`/../PlugIns/Xcode3Core.ideplugin/Contents/SharedSupport/Developer/Library/Xcode/Plug-ins
 * sudo mv HackedBuildSystem.xcspec `xcode-select -print-path`/Platforms/iPhoneSimulator.platform/Developer/Library/Xcode/Specifications
 * See HackedClang.xcplugin/HackedBuildSystem.xcspec @https://raw.githubusercontent.com/kangwang1988/kangwang1988.github.io/master/others/XcodeHacking.
 */

/* The classes below are exported */
#pragma GCC visibility push(default)
using namespace clang;
using namespace std;
using namespace llvm;

namespace ZhMatcher
{
    class ZhMatcherASTVisitor : public RecursiveASTVisitor<ZhMatcherASTVisitor>
    {
    private:
        ASTContext *context;
    public:
        void setContext(ASTContext &context);
        bool VisitExpr(Expr *expr);
        bool VisitStmt(Stmt *s);
        bool preprocessFileAndSrc(Stmt *s,string &filename,string &srcCode,SourceLocation &locFrom,SourceLocation &locTo);
        void postProcess(SourceLocation &locFrom,SourceLocation &locTo,string type,string filename,string srcCode);
    };
    class ZhMatcherASTConsumer : public ASTConsumer
    {
    private:
        ZhMatcherASTVisitor visitor;
        void HandleTranslationUnit(ASTContext &context);
    };
    
    class ZhMatcherASTAction : public PluginASTAction
    {
    public:
        unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &Compiler,llvm::StringRef InFile);
        bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string>& args);
    };
}
#pragma GCC visibility pop
#endif
