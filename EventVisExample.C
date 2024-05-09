void EventVisExample()
{
	//------------------------------------------------
	// Channel mapping
	//------------------------------------------------
	struct chMap
	{
		unsigned short int iL;
		unsigned short int iSMx;
		unsigned short int iSMy;
		unsigned short int iM;
		bool hit;
		double charge;
	};
	chMap map[160];
	for ( Int_t i = 0; i < 160; i ++ )
	{
		map[i] . iL   = i % 2;
		map[i] . iSMx = i / 32;
		map[i] . iSMy = i % 32 / 8;
		map[i] . iM   = i % 8 / 2;
		
		map[i] . hit = kFALSE;
		map[i] . charge = 0.;
	}


	//------------------------------------------------
	// Describe a sample cosmic hit
	// This event is a real example measured at Apr 2024.
	//------------------------------------------------
	map[ 12] . hit = kTRUE;  map[ 12] . charge = 157070.;
	map[ 14] . hit = kTRUE;  map[ 14] . charge = 131106.;
	map[ 42] . hit = kTRUE;  map[ 42] . charge = 178973.;
	map[ 54] . hit = kTRUE;  map[ 54] . charge = 179823.;
	map[ 80] . hit = kTRUE;  map[ 80] . charge = 182279.;
	map[ 82] . hit = kTRUE;  map[ 82] . charge = 118844.;
	map[ 84] . hit = kTRUE;  map[ 84] . charge = 136687.;
	map[120] . hit = kTRUE;  map[120] . charge = 109763.;
	map[124] . hit = kTRUE;  map[124] . charge = 174853.;
	map[126] . hit = kTRUE;  map[126] . charge = 136873.;
	map[154] . hit = kTRUE;  map[154] . charge = 173417.;


	//------------------------------------------------
	// Prepare color palette for charge coloring
	//------------------------------------------------
	const Double_t maxQ = 200000.;
	const Double_t minQ =  50000.;
	const Int_t Number = 2;
	Double_t Red[Number]   = {1., 1.};
	Double_t Green[Number] = {1., 0.};
	Double_t Blue[Number]  = {0., 0.};
	Double_t Stops[Number] = {0., 1.};
	Int_t nb = 256;
	TColor::CreateGradientColorTable(Number, Stops, Red, Green, Blue, nb);
	auto cols = TColor::GetPalette();


	//------------------------------------------------
	// Geometry
	//------------------------------------------------
	// General
	TGeoManager* GM = new TGeoManager("SUBMET", "SUBMET");

	// Define materials
	TGeoMaterial* mat = new TGeoMaterial("mat", 0., 0., 0.);
	TGeoMedium* med = new TGeoMedium("med", 1, mat);

	// Define the transformations
	TGeoTranslation* trMods[4];
	trMods[0] = new TGeoTranslation(  3.444*2,   3.444*2,   0.  );
	trMods[1] = new TGeoTranslation(- 3.444*2,   3.444*2,   0.  );
	trMods[2] = new TGeoTranslation(- 3.444*2, - 3.444*2,   0.  );
	trMods[3] = new TGeoTranslation(  3.444*2, - 3.444*2,   0.  );

	TGeoTranslation* trSMs[20];
	trSMs[ 0] = new TGeoTranslation(  0.   *2,   0.   *2,   0.  );
	trSMs[ 1] = new TGeoTranslation(  0.   *2,  14.289*2,   0.  );
	trSMs[ 2] = new TGeoTranslation(  0.   *2,  28.578*2,   0.  );
	trSMs[ 3] = new TGeoTranslation(  0.   *2,  42.867*2,   0.  );
	trSMs[ 4] = new TGeoTranslation( 14.289*2,   0.   *2,   0.  );
	trSMs[ 5] = new TGeoTranslation( 14.289*2,  14.289*2,   0.  );
	trSMs[ 6] = new TGeoTranslation( 14.289*2,  28.578*2,   0.  );
	trSMs[ 7] = new TGeoTranslation( 14.289*2,  42.867*2,   0.  );
	trSMs[ 8] = new TGeoTranslation( 28.578*2,   0.   *2,   0.  );
	trSMs[ 9] = new TGeoTranslation( 28.578*2,  14.289*2,   0.  );
	trSMs[10] = new TGeoTranslation( 28.578*2,  28.578*2,   0.  );
	trSMs[11] = new TGeoTranslation( 28.589*2,  42.867*2,   0.  );
	trSMs[12] = new TGeoTranslation( 42.867*2,   0.   *2,   0.  );
	trSMs[13] = new TGeoTranslation( 42.867*2,  14.289*2,   0.  );
	trSMs[14] = new TGeoTranslation( 42.867*2,  28.578*2,   0.  );
	trSMs[15] = new TGeoTranslation( 42.867*2,  42.867*2,   0.  );
	trSMs[16] = new TGeoTranslation( 57.156*2,   0.   *2,   0.  );
	trSMs[17] = new TGeoTranslation( 57.156*2,  14.289*2,   0.  );
	trSMs[18] = new TGeoTranslation( 57.156*2,  28.578*2,   0.  );
	trSMs[19] = new TGeoTranslation( 57.156*2,  42.867*2,   0.  );

	TGeoTranslation* trLs[2];
	trLs[0]   = new TGeoTranslation(  0.     ,   0.     ,   0.*2);
	trLs[1]   = new TGeoTranslation(  0.     ,   0.     , 270.*2);

	// Make modules
	TGeoVolume* vMods[160];
	TString sTemp;
	for ( Int_t i = 0; i < 160; i++ )
	{
		sTemp . Form("mod_l%d_x%d_y%d_%d", map[i] . iL, map[i] . iSMx, map[i] . iSMy, map[i] . iM);
		vMods[i] = GM -> MakeBox(sTemp . Data(), med, 5., 5., 150.);
//		vMods[i] -> SetTransparency(8);

		// Coloring
		Int_t iQ = 0;
		if ( map[i] . hit )
		{
			if      ( map[i] . charge > maxQ ) iQ = cols . GetSize() - 1;
			else if ( map[i] . charge < minQ ) iQ = 0;
			else                               iQ = (map[i] . charge - minQ) / (maxQ - minQ) * cols . GetSize();
			vMods[i] -> SetLineColor(cols . At(iQ));
		}
		else
		{
			vMods[i] -> SetLineColor(kBlue);
		}
	}


	// Make supermodules
	TGeoVolume* vSMs[40];
	for ( Int_t i = 0; i < 40; i++ )
	{
		sTemp . Form("sm_l%d_x%d_y%d", i % 2, i / 8, i % 8 / 2);
		vSMs[i] = GM -> MakeBox(sTemp . Data(), med, 14., 14., 150.);
		for ( Int_t j = 0; j < 4; j++ ) vSMs[i] -> AddNode(vMods[8*(i/2) + (i%2) + 2*j], 1, trMods[map[8*(i/2) + (i%2) + 2*j] . iM]);
	}


	// Make layers
	TGeoVolume* vLayers[2];
	for ( Int_t i = 0; i < 2; i++ )
	{
		sTemp . Form("layer_%d", i);
		vLayers[i] = GM -> MakeBox(sTemp . Data(), med, 100., 100., 150.);
		vLayers[i] -> SetVisibility(kFALSE);
		for ( Int_t j = 0; j < 20; j++ ) vLayers[i] -> AddNode(vSMs[i + 2*j], 1, trSMs[j]);
	}


	// Make a full detector
	TGeoVolume* vSUBMET = GM -> MakeBox("submet", med, 100., 100., 500.);
	vSUBMET -> SetVisibility(kFALSE);
	for ( Int_t i = 0; i < 2; i++ ) vSUBMET -> AddNode(vLayers[i], 1, trLs[i]);


	// Make the top container volume
	TGeoVolume* vWorld  = GM -> MakeBox("world" , med, 200., 200., 1000.);
	vWorld -> AddNode(vSUBMET, 1, new TGeoTranslation(0., 0., 0.));
	GM -> SetTopVolume(vWorld);
	
	// Finalize
	GM -> CloseGeometry();


	//------------------------------------------------
	// Event display
	//------------------------------------------------
	auto EM = TEveManager::Create();
	EM -> GetBrowser() -> GetMainFrame() -> MoveResize(5, 30, 2560, 1440);

	TEveGeoTopNode* submet = new TEveGeoTopNode(GM, vWorld -> FindNode("submet_1"));
	EM -> AddGlobalElement(submet);
//	EM -> FullRedraw3D();

//	EM -> GetDefaultGLViewer() -> ResetCameras();
	EM -> GetDefaultGLViewer() -> UpdateScene();
	EM -> GetDefaultGLViewer() -> CurrentCamera() . RotateRad(0., TMath::Pi() / 4.);
	EM -> GetDefaultGLViewer() -> CurrentCamera() . RotateRad(- TMath::Pi() / 6., 0.);
	EM -> GetDefaultGLViewer() -> CurrentCamera() . Zoom(100, 0, 0);
	EM -> GetDefaultGLViewer() -> SavePicture(Form("event.png"));


	return;
}
