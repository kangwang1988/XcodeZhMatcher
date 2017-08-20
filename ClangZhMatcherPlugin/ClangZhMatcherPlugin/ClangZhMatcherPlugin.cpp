/*
 *  ClangZhMatcherPlugin.cpp
 *  ClangZhMatcherPlugin
 *
 *  Created by KyleWong on 26/10/2016.
 *  Copyright © 2016 KyleWong. All rights reserved.
 *
 */

#include <iostream>
#include "ClangZhMatcherPlugin.hpp"

string gSrcRootPath;

namespace ZhMatcher
{
    void ZhMatcherASTVisitor::setContext(ASTContext &context)
    {
        this->context = &context;
    }
    bool ZhMatcherASTVisitor::preprocessFileAndSrc(Stmt *s,string &filename,string &srcCode,SourceLocation &locFrom,SourceLocation &locTo){
        filename.assign(context->getSourceManager().getFilename(s->getSourceRange().getBegin()).str());
        if(filename.find(gSrcRootPath)==string::npos){
            return true;
        }
        SourceLocation sl1(s->getLocStart()),sl2(s->getLocEnd());
        LangOptions lopt;
        SourceLocation sl3(Lexer::getLocForEndOfToken(sl2,0,context->getSourceManager(),lopt));
        const char *from = context->getSourceManager().getCharacterData(sl1);
        int len = context->getSourceManager().getCharacterData(sl3)-context->getSourceManager().getCharacterData(sl1);
        if(!(s->getSourceRange().isValid()&& len>0))
            return true;
        locFrom=sl1;
        locTo=sl2;
        srcCode.assign(from,len);
        return false;
    }
    void ZhMatcherASTVisitor::postProcess(SourceLocation &locFrom,SourceLocation &locTo,string type,string filename,string srcCode){
        if(type.compare("char *")==0 || type.compare("NSString *")==0 || type.find("const char [")==0 || type.find("const char *")==0 ||
           type.compare("string")==0){
            if(containsZh(srcCode)){
                cout<<"[KWLM]"<<"#"<<srcCode<<"#"<<endl;
                
                DiagnosticsEngine &diagEngine = context->getDiagnostics();
                string tempName = string("XLocalized(")+srcCode+")";
                StringRef replacement(tempName);
                FixItHint fixItHint = FixItHint::CreateReplacement(SourceRange(locFrom, locTo), replacement);
                unsigned diagID = diagEngine.getCustomDiagID(DiagnosticsEngine::Warning, "Check if this code sniffet should be I18Ned XX");
                diagEngine.Report(locFrom, diagID).AddFixItHint(fixItHint);
            }
        }
    }
    bool ZhMatcherASTVisitor::VisitStmt(Stmt *s) {
        string filename,srcCode;
        SourceLocation locFrom,locTo;
        bool ret = preprocessFileAndSrc(s,filename,srcCode,locFrom,locTo);
        if(ret)
            return ret;
        string type=s->getStmtClassName();
        if(type.compare("DeclStmt")==0){
            DeclStmt *ds = (DeclStmt *)s;
            Decl *decl = ds->getSingleDecl();
            if(!isa<VarDecl>(decl))
                return true;
            VarDecl *varDecl = (VarDecl *)decl;
            string type=QualType::getAsString(varDecl->getType().split());
            string kindname=decl->getDeclKindName();
            VarDecl::DefinitionKind defKind = varDecl->isThisDeclarationADefinition();
            if(defKind==VarDecl::DeclarationOnly)
                return true;
            postProcess(locFrom,locTo,type,filename,srcCode);
        }
        return true;
    }
    bool ZhMatcherASTVisitor::VisitExpr(Expr *expr) {
        string filename,srcCode;
        SourceLocation locFrom,locTo;
        bool ret = preprocessFileAndSrc((Stmt *)expr,filename,srcCode,locFrom,locTo);
        if(ret)
            return ret;
        string type=QualType::getAsString(expr->getType().split());
        postProcess(locFrom,locTo,type,filename,srcCode);
        return true;
    }
    void ZhMatcherASTConsumer::HandleTranslationUnit(ASTContext &context){
        visitor.setContext(context);
        visitor.TraverseDecl(context.getTranslationUnitDecl());
    }
    unique_ptr<ASTConsumer> ZhMatcherASTAction::CreateASTConsumer(CompilerInstance &Compiler,llvm::StringRef InFile)
    {
        return unique_ptr<ZhMatcherASTConsumer>(new ZhMatcherASTConsumer);
    }
    bool ZhMatcherASTAction::ParseArgs(const CompilerInstance &CI, const std::vector<std::string>& args) {
        size_t cnt = args.size();
        if(cnt == 1){
            string relativePath = args.at(0);
            gSrcRootPath =  absolutePathFromRelative(relativePath);
        }
        return true;
    }
}

static clang::FrontendPluginRegistry::Add<ZhMatcher::ZhMatcherASTAction>
X("ClangZhMatcherPlugin", "ClangZhMatcherPlugin");
