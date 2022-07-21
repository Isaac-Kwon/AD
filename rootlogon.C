{
    // User Defining Area //
    TString libFilename = "libAD";
  
    std::cout << "===================-=====================" <<std::endl<<std::endl;
    std::cout << "ALPIDE Decoder" <<std::endl;
  
    TString DirName = TString(gSystem->GetWorkingDirectory());
    if( !(DirName.EndsWith("/build") || DirName.EndsWith("/build/")) ){
        DirName = DirName + TString("/build/");
    }

    if( !(DirName.EndsWith("/")) ){
        DirName = DirName + TString("/");
    }
  
    TString libIncludePath = TString::Format("%sinclude", DirName.Data());
    TString libLoadPath = TString::Format("%s%s", DirName.Data(), libFilename.Data());

    gInterpreter->AddIncludePath(libIncludePath);
    gInterpreter->Load(libLoadPath, true);
    // gInterpreter->Load(libLoadPath, false);
    gSystem->Load(libLoadPath);
    
    std::cout<<"Include Path: " <<libIncludePath<<std::endl;
    std::cout<<"Library Path: " <<libLoadPath<<std::endl;

    std::cout<<std::endl;

    std::cout<<"AD is loaded for TInterpreter."<<std::endl;
    std::cout<<"Please consider whether you are in same folder with libalpex file."<<std::endl;

    // gInterpreter->LoadMacro("./ADStyle.cpp");
    // SetStyle();

    std::cout <<std::endl<< "===================-=====================" <<std::endl;
}
