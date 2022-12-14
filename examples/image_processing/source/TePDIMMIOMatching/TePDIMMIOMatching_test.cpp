#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIMMIOMatching.hpp>
#include <TePDIUtils.hpp>

#include <TeGTParams.h>
#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TeAgnostic.h>
#include <TeDefines.h>

#include <math.h>

#ifndef M_PI
  #define M_PI       3.14159265358979323846
#endif
#ifndef M_PI_2
  #define M_PI_2     1.57079632679489661923
#endif
#ifndef M_PI_4
  #define M_PI_4     0.785398163397448309616
#endif

void raster2Tiff( 
  const TePDITypes::TePDIRasterPtrType& input_raster_ptr,
  unsigned int raster_channel,
  const std::string& out_file_name,
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr,
  unsigned int tie_points_space )
{   
  TEAGN_TRUE_OR_THROW( ( ! out_file_name.empty() ), 
    "Invalid file name" )
  TEAGN_TRUE_OR_THROW( ( input_raster_ptr->params().nlines_ > 0 ), 
    "Invalid matrix lines" )
  TEAGN_TRUE_OR_THROW( ( input_raster_ptr->params().ncols_ > 0 ), 
    "Invalid matrix cols" )
    
  TeRasterParams params;
  params.setNLinesNColumns( input_raster_ptr->params().nlines_,
    input_raster_ptr->params().ncols_ );
  params.nBands( 3 );
  params.setDataType( TeUNSIGNEDCHAR, -1 );
  params.setPhotometric( TeRasterParams::TeRGB );
  params.decoderIdentifier_ = "TIF";
  params.mode_ = 'c';
  params.fileName_ = out_file_name;
  
  TeRaster out_raster( params );
  TEAGN_TRUE_OR_THROW( out_raster.init(), "Error init raster" );
  double value = 0;
  
  for( int line = 0 ; 
    line < input_raster_ptr->params().nlines_ ; ++line ) {
    for( int col = 0 ; 
      col < input_raster_ptr->params().ncols_ ; 
      ++col ) {
      
      input_raster_ptr->getElement( col, line, value, 
        raster_channel );
        
      TEAGN_TRUE_OR_THROW( out_raster.setElement( col, line, 
        MIN( 254.0, value ), 0 ),
        "Error writing raster" )
      TEAGN_TRUE_OR_THROW( out_raster.setElement( col, line, 
        MIN( 254.0, value ), 1 ),
        "Error writing raster" )
      TEAGN_TRUE_OR_THROW( out_raster.setElement( col, line, 
        MIN( 254.0, value ), 2 ),
        "Error writing raster" )                
    }  
  }
  
  /* Draw tie-points */
  
  if( out_tie_points_ptr.isActive() ) {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      int x = 0;
      int y = 0;
      
      if( tie_points_space == 0 ) {
         x = TeRound( it->pt1.x() );
         y = TeRound( it->pt1.y() );
      } else {
         x = TeRound( it->pt2.x() );
         y = TeRound( it->pt2.y() );
      }
      
      TEAGN_TRUE_OR_THROW( ( x < input_raster_ptr->params().ncols_ ),
        "Invalid maxima column" )
      TEAGN_TRUE_OR_THROW( ( x >= 0 ),
        "Invalid maxima column" )      
      TEAGN_TRUE_OR_THROW( ( y < input_raster_ptr->params().nlines_ ),
        "Invalid maxima line" )
      TEAGN_TRUE_OR_THROW( ( y >= 0 ),
        "Invalid maxima line" )      
      
      TEAGN_TRUE_OR_THROW( out_raster.setElement( x, y, 
        0, 0 ),
        "Error writing raster" )    
      TEAGN_TRUE_OR_THROW( out_raster.setElement( x, y, 
        255, 1 ),
        "Error writing raster" ) 
      TEAGN_TRUE_OR_THROW( out_raster.setElement( x, y, 
        0, 2 ),
        "Error writing raster" ) 
                    
      ++it;
    }
  
  }
}


void rotateRasterClockWize( 
  const TePDITypes::TePDIRasterPtrType& input_raster_ptr,
  const TePDITypes::TePDIRasterPtrType& output_raster_ptr,
  double angle )
{
  TEAGN_TRUE_OR_THROW( input_raster_ptr.isActive(),
    "Invalid input pointer" );
  TEAGN_TRUE_OR_THROW( output_raster_ptr.isActive(),
    "Invalid output pointer" );    
    
  const double input_raster_lines = 
    (double)input_raster_ptr->params().nlines_;
  const double input_raster_cols = 
    (double)input_raster_ptr->params().ncols_;
  const unsigned int input_raster_channels =
    (unsigned int)input_raster_ptr->nBands();
    
  const double last_x_idx = input_raster_cols - 1.0;
  const double last_y_idx = input_raster_lines - 1.0;
    
  const double angle_cos = cos( angle );
  const double angle_sin = sin( angle );
  
  /* Calc new image geometry */
    
  double min_x = MIN( 0.0, ( angle_cos * last_x_idx ) - 
    ( angle_sin * last_y_idx ) );
  min_x = MIN( min_x, ( angle_cos * last_x_idx ) );
  min_x = MIN( min_x, ( -1.0 * angle_sin * last_y_idx ) );
  
  double max_x = MAX( 0.0, ( angle_cos * last_x_idx ) - 
    ( angle_sin * last_y_idx ) );
  max_x = MAX( max_x, ( angle_cos * last_x_idx ) );
  max_x = MAX( max_x, ( -1.0 * angle_sin * last_y_idx ) );
  
  double min_y = MIN( 0.0, ( angle_sin * last_x_idx ) + 
    ( angle_cos * last_y_idx ) );
  min_y = MIN( min_y, ( angle_sin * last_x_idx ) );
  min_y = MIN( min_y, ( angle_cos * last_y_idx ) );
  
  double max_y = MAX( 0.0, ( angle_sin * last_x_idx ) + 
    ( angle_cos * last_y_idx ) );
  max_y = MAX( max_y, ( angle_sin * last_x_idx ) );
  max_y = MAX( max_y, ( angle_cos * last_y_idx ) );
  
  const unsigned int out_lines = (unsigned int) ceil(
    max_y - min_y );
  const unsigned int out_cols = (unsigned int) ceil(
    max_x - min_x );
    
  /* reseting output raster */
  
  TeCoord2D ll_point_input_ref_indexed( min_x,
    max_y );
  TeCoord2D ur_point_input_ref_indexed( max_x,
    min_y );
    
  TeCoord2D ll_point_input_ref = 
    input_raster_ptr->index2Coord( ll_point_input_ref_indexed );
  TeCoord2D ur_point_input_ref = 
    input_raster_ptr->index2Coord( ur_point_input_ref_indexed );    
  
  TeRasterParams new_out_params = output_raster_ptr->params();
  
  new_out_params.boundingBoxLinesColumns(
    ll_point_input_ref.x(), ll_point_input_ref.y(),
    ur_point_input_ref.x(), ur_point_input_ref.y(),
    out_lines, out_cols );
    
  TEAGN_TRUE_OR_THROW( output_raster_ptr->init( new_out_params ),
    "Error reseting output raster" )
  
  /* remapping pixels */
    
  unsigned int curr_out_x = 0;
  unsigned int curr_out_y = 0;
  double value = 0;
  unsigned int curr_in_x = 0;
  unsigned int curr_in_y = 0;
  unsigned int curr_channel = 0;
  
  for( curr_channel = 0 ; curr_channel < input_raster_channels ;
    ++curr_channel ) {
    for( curr_out_y = 0 ; curr_out_y < out_lines ; ++curr_out_y ) {
      for( curr_out_x = 0 ; curr_out_x < out_cols ; ++curr_out_x ) {
        curr_in_x = 
          TeRound( 
            ( angle_cos * ( curr_out_x + min_x ) ) + 
            ( angle_sin * ( curr_out_y + min_y ) )
          );
        curr_in_y = 
          TeRound( 
            ( -1.0 * angle_sin * ( curr_out_x + min_x ) ) + 
            ( angle_cos * ( curr_out_y + min_y ) )
          );
        
        if( input_raster_ptr->getElement( curr_in_x, curr_in_y,
          value, curr_channel ) ) {
        
          output_raster_ptr->setElement( curr_out_x, curr_out_y,
            value, curr_channel );
        } else {
          output_raster_ptr->setElement( curr_out_x, curr_out_y,
            0, curr_channel );        
        }
      }
    }
  }
  
  return;
}


void sameImageAndBoxesTest()
{
  /* Creating parameters */

  TePDIParameters params;
  
  TePDITypes::TePDIRasterPtrType input_image1_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::loadRaster( 
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 
    input_image1_ptr, true ), "Error loading raster" );
  params.SetParameter( "input_image1_ptr" , input_image1_ptr );
  
  params.SetParameter( "input_channel1" , (unsigned int)0 );
    
  TePDITypes::TePDIRasterPtrType input_image2_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::loadRaster( 
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 
    input_image2_ptr, true ), "Error loading raster" );
  params.SetParameter( "input_image2_ptr" , input_image2_ptr ); 
  
  params.SetParameter( "input_channel2" , (unsigned int)0 );       
    
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr( new TeCoordPairVect );
  params.SetParameter( "out_tie_points_ptr" , out_tie_points_ptr );        
  
  params.SetParameter( "matching_method", 
    TePDIMMIOMatching::NormCrossCorrMethod );
  
  // The default geometric transformation parameters will be 
  // changed here for testing purposes.
  TeGTParams gt_params;
  gt_params.transformation_name_ = "affine";
  gt_params.out_rem_strat_ = TeGTParams::LWOutRemotion;
  gt_params.max_dmap_error_ = 2.0;
  gt_params.max_imap_error_ = 2.0;
  params.SetParameter( "gt_params" , gt_params ); 
  
  TeBox input_box1_proj = input_image1_ptr->params().box();
  TeBox input_box1;
  TePDIUtils::MapCoords2RasterIndexes( input_box1_proj, 
    input_image1_ptr, input_box1 );
  params.SetParameter( "input_box1" , input_box1 );
  
  params.SetParameter( "input_box2" , input_box1 );
  
  params.SetParameter( "enable_multi_thread" , (int)1 ); 
  
  // We can skip the geometry filter for this case since the images
  // are the same and the boxes are the same
  params.SetParameter( "skip_geom_filter" , (int)1 );
  
  params.SetParameter( "pixel_x_relation" , (double)1 ); 
  params.SetParameter( "pixel_y_relation" , (double)1 ); 
  
  params.SetParameter( "max_tie_points" , (unsigned int)1000 ); 
  params.SetParameter( "corr_window_width" , (unsigned int)21 );
  params.SetParameter( "moravec_window_width" , (unsigned int)11 );
  
  TeGTParams::pointer out_gt_params_ptr( new TeGTParams );
  params.SetParameter( "out_gt_params_ptr" , out_gt_params_ptr );
  
  TePDIMMIOMatching match_instance; 

  TEAGN_TRUE_OR_THROW( match_instance.Reset( params ),
    "Algorithm reset error" )
    
  TEAGN_LOGMSG( "Algorithm started" )
  
  time_t init_time = clock() / CLOCKS_PER_SEC ;
  
  TEAGN_TRUE_OR_THROW( match_instance.Apply(),
    "Algorithm apply error" )      
    
  time_t end_time = clock() / CLOCKS_PER_SEC;
  
  TEAGN_LOGMSG( "Time elapsed (sameImageAndBoxesTest): " + 
    TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
    " seconds" );
    
  /* Displaying tie-points */
  {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      std::cout << "[" + Te2String( it->pt1.x(),1 ) + " , " +
        Te2String( it->pt1.y(),1 ) + "] -> [" +
        Te2String( it->pt2.x(),1 ) + " , " + 
        Te2String( it->pt2.y(),1 ) + "]" << std::endl;
      
      ++it;
    }
  }
  
  /* Tie-points draw */
  
  raster2Tiff( input_image1_ptr, 0, TEPDIEXAMPLESBINPATH
    "TePDIMMIOMatching_test_sameImageAndBoxesTest_input_image1.tif",
    out_tie_points_ptr, 0 );
  raster2Tiff( input_image2_ptr, 0, TEPDIEXAMPLESBINPATH
    "TePDIMMIOMatching_test_sameImageAndBoxesTest_input_image2.tif",
    out_tie_points_ptr, 1 );    
    
  /* Checking tie-points */
  
  {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      TEAGN_CHECK_EPS( it->pt1.x(), it->pt2.x(), 0, 
        "Invalid tie-point" )
      TEAGN_CHECK_EPS( it->pt1.y(), it->pt2.y(), 0, 
        "Invalid tie-point" )
      
      ++it;
    }
  }
      
  TEAGN_CHECK_EPS( 769, out_tie_points_ptr->size(),
    0, "Invalid tie-points number" )    
}

void sameImageAndBoxesWithMaskTest()
{
  /* Creating parameters */

  TePDIParameters params;
  
  TePDITypes::TePDIRasterPtrType input_image1_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::loadRaster( 
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 
    input_image1_ptr, true ), "Error loading raster" );
  params.SetParameter( "input_image1_ptr" , input_image1_ptr );
  
  params.SetParameter( "input_channel1" , (unsigned int)0 );
    
  TePDITypes::TePDIRasterPtrType input_image2_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::loadRaster( 
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 
    input_image2_ptr, true ), "Error loading raster" );
  params.SetParameter( "input_image2_ptr" , input_image2_ptr ); 
  
  params.SetParameter( "input_channel2" , (unsigned int)0 );       
  
  // Creating the mask image 1 using the input image 1 parameters
  
  TeRasterParams mask1Params = input_image1_ptr->params();
  mask1Params.nBands( 1 );
  mask1Params.setDataType( TeUNSIGNEDCHAR, -1 );
  
  TePDITypes::TePDIRasterPtrType mask_image1_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( mask1Params,
    mask_image1_ptr ), "Mask alloc error" );
  
  {
    for( int line = 0 ; line < mask_image1_ptr->params().nlines_ ;
      ++line )
    {
      for( int col = 0 ; col < mask_image1_ptr->params().ncols_ ;
        ++col )
      {
        TEAGN_TRUE_OR_THROW( mask_image1_ptr->setElement( col, line, 1, 0 ),
          "Error writing mask" );
      }
    }
  }
  
  params.SetParameter( "mask_image1_ptr" , mask_image1_ptr );    
  
  // Creating the mask image 2 using the input image 2 parameters
  
  TeRasterParams mask2Params = input_image2_ptr->params();
  mask2Params.nBands( 1 );
  mask2Params.setDataType( TeUNSIGNEDCHAR, -1 );
  
  TePDITypes::TePDIRasterPtrType mask_image2_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( mask2Params,
    mask_image2_ptr ), "Mask alloc error" );
    
  {
    for( int line = 0 ; line < mask_image2_ptr->params().nlines_ ;
      ++line )
    {
      for( int col = 0 ; col < mask_image2_ptr->params().ncols_ ;
        ++col )
      {
        TEAGN_TRUE_OR_THROW( mask_image2_ptr->setElement( col, line, 1, 0 ),
          "Error writing mask" );
      }
    }
  }
  
  params.SetParameter( "mask_image2_ptr" , mask_image2_ptr );
    
  // Output tie-points vector
    
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr( new TeCoordPairVect );
  params.SetParameter( "out_tie_points_ptr" , out_tie_points_ptr );        
  
  params.SetParameter( "matching_method", 
    TePDIMMIOMatching::NormCrossCorrMethod );
  
  // The default geometric transformation parameters will be 
  // changed here for testing purposes.  
  TeGTParams gt_params;
  gt_params.transformation_name_ = "affine";
  gt_params.out_rem_strat_ = TeGTParams::LWOutRemotion;
  gt_params.max_dmap_error_ = 2.0;
  gt_params.max_imap_error_ = 2.0;
  params.SetParameter( "gt_params" , gt_params ); 
  
  TeBox input_box1_proj = input_image1_ptr->params().box();
  TeBox input_box1;
  TePDIUtils::MapCoords2RasterIndexes( input_box1_proj, 
    input_image1_ptr, input_box1 );
  params.SetParameter( "input_box1" , input_box1 );
  
  params.SetParameter( "input_box2" , input_box1 );
  
  params.SetParameter( "enable_multi_thread" , (int)1 ); 
  
  // We can skip the geometry filter for this case since the images
  // are the same and the boxes are the same
  params.SetParameter( "skip_geom_filter" , (int)1 );
  
  params.SetParameter( "pixel_x_relation" , (double)1 ); 
  params.SetParameter( "pixel_y_relation" , (double)1 ); 
  
  params.SetParameter( "max_tie_points" , (unsigned int)1000 ); 
  params.SetParameter( "corr_window_width" , (unsigned int)21 );
  params.SetParameter( "moravec_window_width" , (unsigned int)11 );
  
  TeGTParams::pointer out_gt_params_ptr( new TeGTParams );
  params.SetParameter( "out_gt_params_ptr" , out_gt_params_ptr );
  
  TePDIMMIOMatching match_instance; 

  TEAGN_TRUE_OR_THROW( match_instance.Reset( params ),
    "Algorithm reset error" )
    
  TEAGN_LOGMSG( "Algorithm started" )
  
  time_t init_time = clock() / CLOCKS_PER_SEC ;
  
  TEAGN_TRUE_OR_THROW( match_instance.Apply(),
    "Algorithm apply error" )      
    
  time_t end_time = clock() / CLOCKS_PER_SEC;
  
  TEAGN_LOGMSG( "Time elapsed (sameImageAndBoxesWithMaskTest): " + 
    TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
    " seconds" );
    
  /* Displaying tie-points */
  {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      std::cout << "[" + Te2String( it->pt1.x(),1 ) + " , " +
        Te2String( it->pt1.y(),1 ) + "] -> [" +
        Te2String( it->pt2.x(),1 ) + " , " + 
        Te2String( it->pt2.y(),1 ) + "]" << std::endl;
      
      ++it;
    }
  }
  
  /* Tie-points draw */
  
  raster2Tiff( input_image1_ptr, 0, TEPDIEXAMPLESBINPATH
    "TePDIMMIOMatching_test_sameImageAndBoxesWithMaskTest_input_image1.tif",
    out_tie_points_ptr, 0 );
  raster2Tiff( input_image2_ptr, 0, TEPDIEXAMPLESBINPATH
    "TePDIMMIOMatching_test_sameImageAndBoxesWithMaskTest_input_image2.tif",
    out_tie_points_ptr, 1 );    
    
  /* Checking tie-points */
  
  {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      TEAGN_CHECK_EPS( it->pt1.x(), it->pt2.x(), 0, 
        "Invalid tie-point" )
      TEAGN_CHECK_EPS( it->pt1.y(), it->pt2.y(), 0, 
        "Invalid tie-point" )
      
      ++it;
    }
  }
      
  TEAGN_CHECK_EPS( 769, out_tie_points_ptr->size(),
    0, "Invalid tie-points number" )    
}

void sameImageDifBoxesTest()
{
  /* Creating parameters */

  TePDIParameters params;
  
  
  TePDITypes::TePDIRasterPtrType input_image1_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::loadRaster( 
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 
    input_image1_ptr, true ), "Error loading raster" );
  params.SetParameter( "input_image1_ptr" , input_image1_ptr );
  
  params.SetParameter( "input_channel1" , (unsigned int)0 );
    
  TePDITypes::TePDIRasterPtrType input_image2_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::loadRaster( 
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 
    input_image2_ptr, true ), "Error loading raster" );
  params.SetParameter( "input_image2_ptr" , input_image2_ptr ); 
  
  params.SetParameter( "input_channel2" , (unsigned int)0 );    
    
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr( new TeCoordPairVect );
  params.SetParameter( "out_tie_points_ptr" , out_tie_points_ptr );        
  
  params.SetParameter( "matching_method", TePDIMMIOMatching::NormCrossCorrMethod );
  
  // The default geometric transformation parameters will be 
  // changed here for testing purposes.  
  TeGTParams gt_params;
  gt_params.transformation_name_ = "affine";
  gt_params.out_rem_strat_ = TeGTParams::LWOutRemotion;
  gt_params.max_dmap_error_ = 0.5;
  gt_params.max_imap_error_ = 0.5;
  params.SetParameter( "gt_params" , gt_params ); 
  
  TeBox input_box1( TeCoord2D( 0, 760 ) , TeCoord2D( 680, 0 ) );
  params.SetParameter( "input_box1" , input_box1 );
  
  TeBox input_box2( 
    TeCoord2D( 190, input_image1_ptr->params().nlines_ - 1 ) , 
    TeCoord2D( input_image1_ptr->params().ncols_ - 1, 227 ) );
  params.SetParameter( "input_box2" , input_box2 );   
  
  //params.SetParameter( "skip_geom_filter" , (int)1 );
  
  params.SetParameter( "pixel_x_relation" , (double)1 ); 
  params.SetParameter( "pixel_y_relation" , (double)1 ); 
  
  params.SetParameter( "enable_multi_thread" , (int)1 ); 
  
  params.SetParameter( "max_tie_points" , (unsigned int)1000 ); 
  params.SetParameter( "corr_window_width" , (unsigned int)21 );
  params.SetParameter( "moravec_window_width" , (unsigned int)11 );
  
  TePDIMMIOMatching match_instance; 

  TEAGN_TRUE_OR_THROW( match_instance.Reset( params ),
    "Algorithm reset error" )
    
  TEAGN_LOGMSG( "Algorithm started" )
  
  time_t init_time = clock() / CLOCKS_PER_SEC;
    
  TEAGN_TRUE_OR_THROW( match_instance.Apply(),
    "Algorithm apply error" )      
    
  time_t end_time = clock() / CLOCKS_PER_SEC;
  
  TEAGN_LOGMSG( "Time elapsed (sameImageDifBoxesTest): " + 
    TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
    " seconds" );
    
  /* Displaying tie-points */
  {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      std::cout << "[" + Te2String( it->pt1.x(),1 ) + " , " +
        Te2String( it->pt1.y(),1 ) + "] -> [" +
        Te2String( it->pt2.x(),1 ) + " , " + 
        Te2String( it->pt2.y(),1 ) + "]" << std::endl;
      
      ++it;
    }
  }
  
  /* Tie-points draw */
  
  raster2Tiff( input_image1_ptr, 0, TEPDIEXAMPLESBINPATH
    "TePDIMMIOMatching_test_sameImageDifBoxesTest_input_image1.tif",
    out_tie_points_ptr, 0 );
  raster2Tiff( input_image2_ptr, 0, TEPDIEXAMPLESBINPATH
    "TePDIMMIOMatching_test_sameImageDifBoxesTest_input_image2.tif",
    out_tie_points_ptr, 1 );    
    
  /* Checking tie-points */
  
      
  TEAGN_CHECK_EPS( 234, out_tie_points_ptr->size(),
    0, "Invalid tie-points number" )    
  
  {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      TEAGN_CHECK_EPS( it->pt1.x(), it->pt2.x(), 0, 
        "Invalid tie-point" )
      TEAGN_CHECK_EPS( it->pt1.y(), it->pt2.y(), 0, 
        "Invalid tie-point" )
      
      ++it;
    }
  }
}


void halfsampledImageTest()
{
  /* Creating parameters */

  TePDIParameters params;
  
  TePDITypes::TePDIRasterPtrType input_image1_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::loadRaster( 
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 
    input_image1_ptr, true ), "Error loading raster" );
  params.SetParameter( "input_image1_ptr" , input_image1_ptr );
  
  params.SetParameter( "input_channel1" , (unsigned int)0 );
    
  TePDITypes::TePDIRasterPtrType input_image2_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::loadRaster( 
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_contraste_halfsampled.tif" ), 
    input_image2_ptr, true ), "Error loading raster" );
  params.SetParameter( "input_image2_ptr" , input_image2_ptr ); 
  
  params.SetParameter( "input_channel2" , (unsigned int)0 );    
    
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr( new TeCoordPairVect );
  params.SetParameter( "out_tie_points_ptr" , out_tie_points_ptr );        
  
  params.SetParameter( "matching_method", TePDIMMIOMatching::NormCrossCorrMethod );
  
  // The default geometric transformation parameters will be 
  // changed here for testing purposes.  
  TeGTParams gt_params;
  gt_params.transformation_name_ = "affine";
  gt_params.out_rem_strat_ = TeGTParams::LWOutRemotion;
  gt_params.max_dmap_error_ = 2.0;
  gt_params.max_imap_error_ = 2.0;
  params.SetParameter( "gt_params" , gt_params ); 
  
  TeBox input_box1( TeCoord2D( 122.0, 698.0 ), 
    TeCoord2D( 730.0, 138.0 ) );
  params.SetParameter( "input_box1" , input_box1 );
  
  TeBox input_box2( TeCoord2D( 110.0, 305.0 ), 
    TeCoord2D( 385.0, 50.0 ) );
  params.SetParameter( "input_box2" , input_box2 );   
  
  params.SetParameter( "enable_multi_thread" , (int)1 ); 
//  params.SetParameter( "skip_geom_filter" , (int)1 );
  params.SetParameter( "corr_window_width" , (unsigned int)21 );
  params.SetParameter( "moravec_window_width" , (unsigned int)11 );
  
  params.SetParameter( "pixel_x_relation" , (double)0.5 ); 
  params.SetParameter( "pixel_y_relation" , (double)0.5 ); 
  
  params.SetParameter( "max_tie_points" , (unsigned int)1000 ); 
  
  TePDIMMIOMatching match_instance; 

  TEAGN_TRUE_OR_THROW( match_instance.Reset( params ),
    "Algorithm reset error" )
    
  TEAGN_LOGMSG( "Algorithm started" )
  
  time_t init_time = clock() / CLOCKS_PER_SEC;
    
  TEAGN_TRUE_OR_THROW( match_instance.Apply(),
    "Algorithm apply error" )      
    
  time_t end_time = clock() / CLOCKS_PER_SEC;
  
  TEAGN_LOGMSG( "Time elapsed (halfsampledImageTest): " + 
    TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
    " seconds" );
    
  /* Displaying tie-points */
  
  TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
  TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
  
  while( it != it_end ) {
    std::cout << "[" + Te2String( it->pt1.x(),1 ) + " , " +
      Te2String( it->pt1.y(),1 ) + "] -> [" +
      Te2String( it->pt2.x(),1 ) + " , " + 
      Te2String( it->pt2.y(),1 ) + "]" << std::endl;
    
    ++it;
  }
  
  /* Tie-points draw */
  
  raster2Tiff( input_image1_ptr, 0, TEPDIEXAMPLESBINPATH
    "TePDIMMIOMatching_test_halfsampledImageTest_input_image1.tif",
    out_tie_points_ptr, 0 );
  raster2Tiff( input_image2_ptr, 0, TEPDIEXAMPLESBINPATH
    "TePDIMMIOMatching_test_halfsampledImageTest_input_image2.tif",
    out_tie_points_ptr, 1 );  
    
  TEAGN_WATCH( (unsigned int)out_tie_points_ptr->size() );
  TEAGN_CHECK_EPS( 56, out_tie_points_ptr->size(),
    0, "Invalid tie-points number" )         
}


void halfsampledImageTest2()
{
  /* Creating parameters */

  TePDIParameters params;
  
  TePDITypes::TePDIRasterPtrType input_image1_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::loadRaster( 
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_contraste_halfsampled.tif" ), 
    input_image1_ptr, true ), "Error loading raster" );
  params.SetParameter( "input_image1_ptr" , input_image1_ptr );
  
  params.SetParameter( "input_channel1" , (unsigned int)0 );
    
  TePDITypes::TePDIRasterPtrType input_image2_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::loadRaster( 
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 
    input_image2_ptr, true ), "Error loading raster" );
  params.SetParameter( "input_image2_ptr" , input_image2_ptr ); 
  
  params.SetParameter( "input_channel2" , (unsigned int)0 );   
   
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr( new TeCoordPairVect );
  params.SetParameter( "out_tie_points_ptr" , out_tie_points_ptr );        
  
  params.SetParameter( "matching_method", TePDIMMIOMatching::NormCrossCorrMethod );
  
  // The default geometric transformation parameters will be 
  // changed here for testing purposes.  
  TeGTParams gt_params;
  gt_params.transformation_name_ = "affine";
  gt_params.out_rem_strat_ = TeGTParams::LWOutRemotion;
  gt_params.max_dmap_error_ = 2.0;
  gt_params.max_imap_error_ = 2.0;
  params.SetParameter( "gt_params" , gt_params ); 
  
  TeBox input_box1( TeCoord2D( 110.0, 305.0 ), 
    TeCoord2D( 385.0, 50.0 ) );    
  params.SetParameter( "input_box1" , input_box1 );
  
  TeBox input_box2( TeCoord2D( 122.0, 698.0 ), 
    TeCoord2D( 730.0, 138.0 ) );    
  params.SetParameter( "input_box2" , input_box2 );   
  
  params.SetParameter( "enable_multi_thread" , (int)1 ); 
//  params.SetParameter( "skip_geom_filter" , (int)1 );
  
  params.SetParameter( "pixel_x_relation" , (double)2 ); 
  params.SetParameter( "pixel_y_relation" , (double)2 ); 
  
  params.SetParameter( "max_tie_points" , (unsigned int)1000 );
  params.SetParameter( "corr_window_width" , (unsigned int)21 );
  params.SetParameter( "moravec_window_width" , (unsigned int)11 );
  
  TePDIMMIOMatching match_instance; 

  TEAGN_TRUE_OR_THROW( match_instance.Reset( params ),
    "Algorithm reset error" )
    
  TEAGN_LOGMSG( "Algorithm started" )
  
  time_t init_time = clock() / CLOCKS_PER_SEC;
    
  TEAGN_TRUE_OR_THROW( match_instance.Apply(),
    "Algorithm apply error" )      
    
  time_t end_time = clock() / CLOCKS_PER_SEC;
  
  TEAGN_LOGMSG( "Time elapsed (halfsampledImageTest2): " + 
    TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
    " seconds" );
    
  /* Displaying tie-points */
  
  TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
  TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
  
  while( it != it_end ) {
    std::cout << "[" + Te2String( it->pt1.x(),1 ) + " , " +
      Te2String( it->pt1.y(),1 ) + "] -> [" +
      Te2String( it->pt2.x(),1 ) + " , " + 
      Te2String( it->pt2.y(),1 ) + "]" << std::endl;
    
    ++it;
  }
  
  /* Tie-points draw */
  
  raster2Tiff( input_image1_ptr, 0, TEPDIEXAMPLESBINPATH
    "TePDIMMIOMatching_test_halfsampledImageTest2_input_image1.tif",
    out_tie_points_ptr, 0 );
  raster2Tiff( input_image2_ptr, 0, TEPDIEXAMPLESBINPATH
    "TePDIMMIOMatching_test_halfsampledImageTest2_input_image2.tif",
    out_tie_points_ptr, 1 );    
    
  TEAGN_WATCH( (unsigned int)out_tie_points_ptr->size() );
  TEAGN_CHECK_EPS( 56, out_tie_points_ptr->size(),
    0, "Invalid tie-points number" )     
}


void halfsampledRotadedImageTest()
{
  /* Open image 1 */

  TePDIParameters params;
  
  TePDITypes::TePDIRasterPtrType input_image1_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::loadRaster( 
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 
    input_image1_ptr, true ), "Error loading raster" );
  params.SetParameter( "input_image1_ptr" , input_image1_ptr );
  
  params.SetParameter( "input_channel1" , (unsigned int)0 );
  
  /* Creating the half-sampled-rotated image 2 */
    
  TePDITypes::TePDIRasterPtrType input_disk_image2_ptr( new TeRaster(
    std::string( 
    TEPDIEXAMPLESRESPATH "cbers_b2_crop_contraste_halfsampled.tif" ), 
    'r' ) );
  TEAGN_TRUE_OR_THROW( input_disk_image2_ptr->init(), 
    "Unable to init input_image2_ptr" );
      
  TePDITypes::TePDIRasterPtrType input_image2_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( 
    input_image2_ptr, input_disk_image2_ptr->params(), 
    TePDIUtils::TePDIUtilsAutoMemPol ),
    "Error allocating raster memory" );
    
  rotateRasterClockWize( input_disk_image2_ptr, input_image2_ptr, M_PI_4 );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( input_image2_ptr, 
    TEPDIEXAMPLESBINPATH "TePDIMMIOMatching_test_rotatedimage.tif", 
    TeUNSIGNEDCHAR ),
    "Error saving rotated image" );
      
  params.SetParameter( "input_image2_ptr" , input_image2_ptr ); 
  
  /* Set the other parameters */
  
  params.SetParameter( "input_channel2" , (unsigned int)0 );       
    
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr( new TeCoordPairVect );
  params.SetParameter( "out_tie_points_ptr" , out_tie_points_ptr );        
  
  params.SetParameter( "matching_method", 
    TePDIMMIOMatching::NormCrossCorrMethod );
  
  TeBox input_box1( TeCoord2D( 122.0, 698.0 ), 
    TeCoord2D( 730.0, 138.0 ) );
  params.SetParameter( "input_box1" , input_box1 );
  
  TeBox input_box2( TeCoord2D( 173.0, 447.0 ), 
    TeCoord2D( 434.0, 153.0 ) );
  params.SetParameter( "input_box2" , input_box2 );   
  
  params.SetParameter( "enable_multi_thread" , (int)1 ); 
//  params.SetParameter( "skip_geom_filter" , (int)1 );
  params.SetParameter( "corr_window_width" , (unsigned int)21 );
  params.SetParameter( "moravec_window_width" , (unsigned int)11 );
  
  params.SetParameter( "pixel_x_relation" , (double)0.5 ); 
  params.SetParameter( "pixel_y_relation" , (double)0.5 ); 
  
  // These rasters are for test purposes only
  TePDITypes::TePDIRasterPtrType out_maximas_image1_ptr;
  TePDITypes::TePDIRasterPtrType out_maximas_image2_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster(
    out_maximas_image1_ptr, 1, 1, 1, false, TeUNSIGNEDCHAR, 0 ),
    "Raster allocation error" )
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster(
    out_maximas_image2_ptr, 1, 1, 1, false, TeUNSIGNEDCHAR, 0 ),
    "Raster allocation error" )
  params.SetParameter( "out_maximas_image1_ptr" , out_maximas_image1_ptr );
  params.SetParameter( "out_maximas_image2_ptr" , out_maximas_image2_ptr );   
  
  TePDIMMIOMatching match_instance; 

  TEAGN_TRUE_OR_THROW( match_instance.Reset( params ),
    "Algorithm reset error" )
    
  TEAGN_LOGMSG( "Algorithm started" )
  
  time_t init_time = clock() / CLOCKS_PER_SEC;
    
  TEAGN_TRUE_OR_THROW( match_instance.Apply(),
    "Algorithm apply error" )      
    
  time_t end_time = clock() / CLOCKS_PER_SEC;
  
  TEAGN_LOGMSG( "Time elapsed (halfsampledRotadedImageTest): " + 
    TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
    " seconds" );
    
  // Generate the output tif maximas images
  // These rasters are for test purposes only
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( out_maximas_image1_ptr,
    TEPDIEXAMPLESBINPATH "TePDIMMIOMatching_halfsampledRotadedImageTest_maximas_image1.tif",
    true ), "Error generating TIF" )
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( out_maximas_image2_ptr,
    TEPDIEXAMPLESBINPATH "TePDIMMIOMatching_halfsampledRotadedImageTest_maximas_image2.tif",
    true ), "Error generating TIF" )    
    
  /* Displaying tie-points */
  
  TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
  TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
  
  while( it != it_end ) {
    std::cout << "[" + Te2String( it->pt1.x(),1 ) + " , " +
      Te2String( it->pt1.y(),1 ) + "] -> [" +
      Te2String( it->pt2.x(),1 ) + " , " + 
      Te2String( it->pt2.y(),1 ) + "]" << std::endl;
    
    ++it;
  }
  
  /* Tie-points draw */
  
  raster2Tiff( input_image1_ptr, 0, TEPDIEXAMPLESBINPATH
    "TePDIMMIOMatching_test_halfsampledRotadedImageTest_input_image1.tif",
    out_tie_points_ptr, 0 );
  raster2Tiff( input_image2_ptr, 0, TEPDIEXAMPLESBINPATH
    "TePDIMMIOMatching_test_halfsampledRotadedImageTest_input_image2.tif",
    out_tie_points_ptr, 1 );   
    
  TEAGN_WATCH( (unsigned int)out_tie_points_ptr->size() );
  TEAGN_CHECK_EPS( 22, out_tie_points_ptr->size(),
    0, "Invalid tie-points number" )
}

// Testing two distinct images with different resolutions
void HRCxCCDTest()
{
  /* Creating parameters */

  TePDIParameters params;
  
  TePDITypes::TePDIRasterPtrType input_image1_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::loadRaster( 
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_hrc_crop.tif" ), 
    input_image1_ptr, true ), "Error loading raster" );
  params.SetParameter( "input_image1_ptr" , input_image1_ptr );
  
  params.SetParameter( "input_channel1" , (unsigned int)0 );
    
  TePDITypes::TePDIRasterPtrType input_image2_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::loadRaster( 
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 
    input_image2_ptr, true ), "Error loading raster" );
  params.SetParameter( "input_image2_ptr" , input_image2_ptr ); 
  
  params.SetParameter( "input_channel2" , (unsigned int)2 );    
    
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr( new TeCoordPairVect );
  params.SetParameter( "out_tie_points_ptr" , out_tie_points_ptr );        
  
  params.SetParameter( "matching_method", TePDIMMIOMatching::NormCrossCorrMethod );
  
  // The default geometric transformation parameters will be 
  // changed here for testing purposes.  
  TeGTParams gt_params;
  gt_params.transformation_name_ = "affine";
  gt_params.out_rem_strat_ = TeGTParams::LWOutRemotion;
  gt_params.max_dmap_error_ = 2; // max error over img 2
  gt_params.max_imap_error_ = 16; // max error over img 1
  params.SetParameter( "gt_params" , gt_params ); 
  
  params.SetParameter( "pixel_x_relation" , input_image1_ptr->params().resx_ /
    input_image2_ptr->params().resx_ ); 
  params.SetParameter( "pixel_y_relation" , input_image1_ptr->params().resy_ /
    input_image2_ptr->params().resy_ ); 
  
  params.SetParameter( "enable_multi_thread" , (int)1 ); 
  
  params.SetParameter( "max_tie_points" , (unsigned int)1000 ); 
  
  TePDIMMIOMatching match_instance; 

  TEAGN_TRUE_OR_THROW( match_instance.Reset( params ),
    "Algorithm reset error" )
    
  TEAGN_LOGMSG( "Algorithm started" )
  
  time_t init_time = clock() / CLOCKS_PER_SEC;
    
  TEAGN_TRUE_OR_THROW( match_instance.Apply(),
    "Algorithm apply error" )      
    
  time_t end_time = clock() / CLOCKS_PER_SEC;
  
  TEAGN_LOGMSG( "Time elapsed (sameImageDifBoxesTest): " + 
    TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
    " seconds" );
    
  /* Displaying tie-points */
  {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      std::cout << "[" + Te2String( it->pt1.x(),1 ) + " , " +
        Te2String( it->pt1.y(),1 ) + "] -> [" +
        Te2String( it->pt2.x(),1 ) + " , " + 
        Te2String( it->pt2.y(),1 ) + "]" << std::endl;
      
      ++it;
    }
  }
  
  /* Tie-points draw */
  
  raster2Tiff( input_image1_ptr, 0, TEPDIEXAMPLESBINPATH
    "TePDIMMIOMatching_HRCxCCDTest_input_image1.tif",
    out_tie_points_ptr, 0 );
  raster2Tiff( input_image2_ptr, 0, TEPDIEXAMPLESBINPATH
    "TePDIMMIOMatching_HRCxCCDTest_input_image2.tif",
    out_tie_points_ptr, 1 );    
    
  /* Checking tie-points */
  
      
  TEAGN_CHECK_EPS( 22, out_tie_points_ptr->size(),
    0, "Invalid tie-points number" )    
}

int main()
{
  TEAGN_LOGMSG( "Test started." );

  TeStdIOProgress pi;
  TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );     
  
  time_t init_time = clock() / CLOCKS_PER_SEC;
  
  sameImageAndBoxesTest();
  sameImageAndBoxesWithMaskTest();
  sameImageDifBoxesTest();
  halfsampledImageTest();
  halfsampledImageTest2();
  halfsampledRotadedImageTest();
  HRCxCCDTest();
  
  time_t end_time = clock() / CLOCKS_PER_SEC;
  
  TEAGN_LOGMSG( "Total elapsed time: " + 
    TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
    " seconds" );    

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}

