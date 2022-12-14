/*
TerraLib - a library for developing GIS applications.
Copyright  2001, 2002, 2003 INPE and Tecgraf/PUC-Rio.

This code is part of the TerraLib library.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

You should have received a copy of the GNU Lesser General Public
License along with this library.

The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular
purpose. The library provided hereunder is on an "as is" basis, and the
authors have no obligation to provide maintenance, support, updates,
enhancements, or modifications.
In no event shall INPE be held liable to any party
for direct, indirect, special, incidental, or consequential damages arising
out of the use of this library and its documentation.
*/

#ifndef TEPDIMATRIX_HPP
  #define TEPDIMATRIX_HPP
  
  #include <TeAgnostic.h>
  #include <TeSharedPtr.h>
  #include <TeTempFilesRemover.h>
  #include <TeUtils.h>
  
  #include <vector>
  #include <string>
  

  /**
   * @class TePDIMatrix
   * @brief This is the template class to deal with a generic matrix.
   * @author Emiliano F. Castejon <castejon@dpi.inpe.br>
   * @ingroup PDIAux
   * @example TePDIMatrix_test.cpp
   */
  template< class ElementType >
  class TePDIMatrix {
    public :
      /** @typedef TeSharedPtr< TePDIMatrix< ElementType > > pointer 
          Type definition for a instance pointer */
      typedef TeSharedPtr< TePDIMatrix< ElementType > > pointer;
      
      /**
       * @brief Memory polycy.
       */
      enum MemoryPolicy 
      {
        /**
         * Automatic memory policy ( Try to use RAM or DISK, if there is no 
         * avaliable RAM ) -
         * DO NOT USE AutoMemPol FOR COMPLEX DATA TYPES !
         * 
         */
        AutoMemPol,
        /**
         * RAM memory policy.
         */
        RAMMemPol,
        /**
         * Disk memory policy ( virtual mapped memory ) -
         *  DO NOT USE DiskMemPol FOR COMPLEX DATA TYPES !
         */
        DiskMemPol
      };
      
      TePDIMatrix();      

      TePDIMatrix( const TePDIMatrix< ElementType >& external );

      virtual ~TePDIMatrix();
      
      /**
       * @brief Reset (clear) the active instance data.
       * @note Other parameters will not be changed.
       *
       * @param memoryPolicy Memory policy.
       */
      void Reset();      
      
      /**
       * @brief Reset (clear) the active instance data and update its
       * internal parameters folowing the the new supplied parameters.
       * @note Other parameters will not be changed.
       *
       * @param memoryPolicy Memory policy.
       */
      void Reset( MemoryPolicy memoryPolicy );       
      
      /**
       * @brief Reset (clear) the active instance data and update its
       * internal parameters folowing the the new supplied parameters.
       * @note Other parameters will not be changed.
       *
       * @param lines Number of lines.
       * @param columns Number of columns.
       * @return true if OK, false on error.
       */
      bool Reset( unsigned int lines, unsigned int columns );      

      /**
       * @brief Reset (clear) the active instance data and update its
       * internal parameters folowing the the new supplied parameters.
       * @note Other parameters will not be changed.
       *
       * @param lines Number of lines.
       * @param columns Number of columns.
       * @param memoryPolicy Memory policy.
       * @return true if OK, false on error.
       */
      bool Reset( unsigned int lines, unsigned int columns,
        MemoryPolicy memoryPolicy );        
        
      /**
       * @brief Reset (clear) the active instance data and update its
       * internal parameters folowing the the new supplied parameters.
       * @note Other parameters will not be changed.
       *
       * @param lines Number of lines.
       * @param columns Number of columns.
       * @param memoryPolicy Memory policy.
       * @param maxTmpFileSize Max temp file size.
       * @param maxMemPercentUsage The max amount of free memory to use 
       * when necessary.
       * @return true if OK, false on error.
       */
      bool Reset( unsigned int lines, unsigned int columns,
        MemoryPolicy memoryPolicy, unsigned long int maxTmpFileSize,
        unsigned char maxMemPercentUsage ); 
        
      /**
       * @brief Clear all allocated resources and go back to the initial
       * default parameters.
       */      
      void clear();            
      
      /**
       * @brief The number of current matrix lines.
       *
       * @return The number of current matrix lines.
       */
      unsigned int GetLines() const;
      
      /**
       * @brief The number of current matrix columns.
       *
       * @return The number of current matrix columns
       */
      unsigned int GetColumns() const;
      
      /**
       * @brief Empty Matrix verification.
       *
       * @return true if the matrix is empty.
       */
      bool IsEmpty() const;

      /**
       * @brief Operator = overload.
       *
       * @note The external memory policy will be used as reference.
       *
       * @param external External instance reference.
       * @return A reference to the current matrix.
       */
      const TePDIMatrix< ElementType >& operator=( 
        const TePDIMatrix< ElementType >& external );

      /**
       * @brief Operator () overload.
       *
       * @param line Line number.
       * @param column Column number.
       * @return A reference to the required element.
       */
      inline ElementType& operator()( const unsigned int& line, 
        const unsigned int& column )
      {
        TEAGN_DEBUG_CONDITION( ( column < totalColumns_ ),
          "Invalid columns" )
            
        return getScanLine( line )[ column ];      
      };
      
      /**
       * @brief Operator () overload.
       *
       * @param line Line number.
       * @param column Column number.
       * @return A const reference to the required element.
       */
      inline const ElementType& operator()( const unsigned int& line, 
        const unsigned int& column ) const
      {
        TEAGN_DEBUG_CONDITION( ( column < totalColumns_ ),
          "Invalid columns" )
          
        return getScanLine( line )[ column ] ;      
      }; 
      
      /**
       * @brief Operator [] overload.
       *
       * @param line Line number.
       * @param column Column number.
       * @return A pointer to the required line.
       * @note The returned pointer is garanteed to 
       * be valid until an acess to another line occurs.
       * @note Concurrent thread access to this method is guaranteed
       * if RAMMemPol policy method is used.
       */
      inline ElementType* operator[]( const unsigned int& line )
      {
        return getScanLine( line );
      };
      
      /**
       * @brief Operator [] overload.
       *
       * @param line Line number.
       * @param column Column number.
       * @return A pointer to the required line.
       * @note The returned pointer is garanteed to 
       * be valid until an acess to another line occurs.
       * @note Concurrent thread access to this method is guaranteed
       * if RAMMemPol policy method is used.       
       */
      inline ElementType const* operator[]( const unsigned int& line ) const
      {
        return getScanLine( line );
      };      
      
      /**
       * @brief Returns the current memory policy.
       * @return The current memory policy.
       */      
      MemoryPolicy getMemPolicy() const;
      
      /**
       * @brief Returns the current maximum temporary disk file size.
       * @return Returns the current maximum temporary disk file size.
       */            
      unsigned long int getMaxTmpFileSize() const; 
            
    protected :
    
      /**
       * @brief Disk lines infor node.
       */         
      class DiskLinesInfoNode
      {
        public :
          
          FILE* filePtr_;
          unsigned int fileOff_;
          
          DiskLinesInfoNode()
          {
            filePtr_ = 0;
            fileOff_ = 0;
          }
          
          ~DiskLinesInfoNode() {};
      };
      
      /**
       * @brief Disk lines vector type definition.
       */      
      typedef std::vector< DiskLinesInfoNode > DiskLinesVecT;
      
      /**
       * @typedef std::vector< std::pair< FILE*, unsigned int > >
       * Openend disk files data vector type.
       */         
      typedef std::vector< std::pair< FILE*, std::string > > 
        OpenDiskFilesVecT;         
      
      /**
       * @brief Max bytes per temp file (for swapped tiles only, default: 2GB).
       */
      unsigned long int maxTmpFileSize_;      
      
      /**
       * @brief the max amount of free memory to use when necessary
       * (default:50).
       */        
      unsigned char maxMemPercentUsage_;
      
      /**
       * @brief The total lines number (default:0).
       */
      unsigned int totalLines_;
      
      /**
       * @brief The total columns number (default:0).
       */
      unsigned int totalColumns_;     
      
      /**
       * @brief The used memory policy (default:RAMMemPol).
       */
      MemoryPolicy memoryPolicy_;
      
      /**
       * @brief The line size (bytes, default:0).
       */     
      unsigned int lineSize_;      

      /**
       * @brief A vector with pointers to all lines.
       */
      mutable std::vector< ElementType* > allLinesPtrsVec_;     
      
      /**
       * @brief A vector with open disk files info.
       */      
      OpenDiskFilesVecT openDiskFilesVec_;
      
      /**
       * @brief Disk lines vector.
       */      
      mutable DiskLinesVecT diskLinesVec_;      
      
      /**
       * @brief The indexes inside allTilesPtrsVec_ of all RAM lines.
       */        
      mutable std::vector< unsigned int > ramLinesIndexesVec_;
      
      /**
       * @brief The index inside ramLinesIndexesVec_ of the next RAM
       * line index that will be swapped to disk when a disk line
       * is required.
       */        
      mutable unsigned int nextSwapLineRamLinesIndexesVecIdx_;
      
      /**
       * @brief A pointer to the auxiliar line used when swapping
       * data to/from disk.
       */              
      mutable ElementType* swapLinePtr_;      

         
      // temp variables used by the method getScanLine
      mutable ElementType* getScanLine_linePtr_;
      mutable unsigned int getScanLine_swapLineIdx_;
     
      /**
       * @brief Reset the internal variables to the initial state.
       */      
      void init();
      
      /**
       * @brief Allocate disk lines.
       * @param startingLineIdx Starting line index.
       * @return true if OK, false on errors.
       */    
      bool allocateDiskLines( unsigned int startingLineIdx );      
      
      /**
       * @brief Returns a pointer to the required line.
       *
       * @param line Line number.
       * @param column Column number.
       * @return A pointer to the required line.
       * @note The returned pointer is garanteed to 
       * be valid until an acess to another line occurs.
       * @note Concurrent thread access to this method is guaranteed
       * if RAMMemPol policy method is used.       
       */
      ElementType* getScanLine( const unsigned int& line ) const;
        
      /**
       * @brief Create a new disk file.
       * @param filename The file name.
       * @param size The file size.
       * @param fileptr The file pointer.
       * @return true if OK. false on errors.
       */
      bool createNewDiskFile( unsigned long int size,
        const std::string& filename, FILE** fileptr ) const;      
  };

  template< class ElementType >
  void TePDIMatrix< ElementType >::init()
  {
    maxTmpFileSize_ = 2ul * 1024ul * 1024ul * 1024ul;;
    maxMemPercentUsage_ = 50;
    totalLines_ = 0;
    totalColumns_ = 0;  
    memoryPolicy_ = RAMMemPol;
    lineSize_ = 0;
    nextSwapLineRamLinesIndexesVecIdx_ = 0;
    getScanLine_linePtr_ = 0;
    getScanLine_swapLineIdx_ = 0;
    swapLinePtr_ = 0;
  }

  template< class ElementType >
  TePDIMatrix< ElementType >::TePDIMatrix()
  {
    init();
  }    
  
  template< class ElementType >
  TePDIMatrix< ElementType >::TePDIMatrix( 
    const TePDIMatrix< ElementType >& external )
  {
    init();
    
    operator=( external );
  }

  template< class ElementType >
    TePDIMatrix< ElementType >::~TePDIMatrix()
  {
    clear();
  }
  
  template< class ElementType >
  void TePDIMatrix< ElementType >::Reset()
  {
    Reset( 0, 0, memoryPolicy_, maxTmpFileSize_, maxMemPercentUsage_ );
  }  
  
  template< class ElementType >
  void TePDIMatrix< ElementType >::Reset( MemoryPolicy memoryPolicy )
  {
    Reset( 0, 0, memoryPolicy, maxTmpFileSize_, maxMemPercentUsage_ );
  }   
  
  template< class ElementType >
  bool TePDIMatrix< ElementType >::Reset( unsigned int lines, 
    unsigned int columns )
  {
    return Reset( lines, columns, memoryPolicy_, maxTmpFileSize_,
      maxMemPercentUsage_ );
  } 
   
  template< class ElementType >
  bool TePDIMatrix< ElementType >::Reset( unsigned int lines, 
    unsigned int columns, MemoryPolicy memoryPolicy )
  {
    return Reset( lines, columns, memoryPolicy, maxTmpFileSize_, 
      maxMemPercentUsage_ );
  }

  template< class ElementType >
  bool TePDIMatrix< ElementType >::Reset( unsigned int lines, 
    unsigned int columns, MemoryPolicy memoryPolicy,
    unsigned long int maxTmpFileSize,
    unsigned char maxMemPercentUsage )
  {
    clear();
    
    // Updating the global vars
    
    maxTmpFileSize_ = maxTmpFileSize;
    maxMemPercentUsage_ = maxMemPercentUsage;
    memoryPolicy_ = memoryPolicy;
    
    /* Update the old buffer if necessary */
    
    if( ( lines > 0 ) || ( columns > 0 ) )
    {
      try
      {
        // Updating the global vars
        
        totalLines_ = lines;
        totalColumns_ = columns;    
        lineSize_ = sizeof( ElementType ) * totalColumns_;
              
        // Allocating tiles
        
        allLinesPtrsVec_.resize( totalLines_, 0 );
      
        if( memoryPolicy_ == RAMMemPol )
        {
          for( unsigned int allLinesPtrsVecIdx = 0 ; allLinesPtrsVecIdx <
            totalLines_ ; ++allLinesPtrsVecIdx )
          {
            allLinesPtrsVec_[ allLinesPtrsVecIdx ] = 
              new ElementType[ totalColumns_ ];
              
            if( allLinesPtrsVec_[ allLinesPtrsVecIdx ] == 0 )
            {
              clear();
            
              TEAGN_LOG_AND_RETURN( "Tiles allocation error" )
            }
          }
        }
        else
        { // AutoMemPol, DiskMemPol
        
          // Allocating the swap line pointer
          
          swapLinePtr_ = new ElementType[ totalColumns_ ];
          
          if( swapLinePtr_ == 0 )
          {
            clear();
          
            TEAGN_LOG_AND_RETURN( "Swap line allocation error" )
          }          
        
          // Defining the number of max RAM lines
          
          unsigned int maxRAMLines = 1;
          
          if( memoryPolicy_ == AutoMemPol )
          {
            // Defining the max number of RAM tiles
            
            const double totalPhysMem = (double)TeGetTotalPhysicalMemory();
            
            const double totalVMem = (double)TeGetTotalVirtualMemory();
            
            const double totalMem = ( ((double)maxMemPercentUsage_) / 100.0 ) *
              MIN( totalVMem, totalPhysMem );
              
            const double freeMem = MIN( totalMem, (double)TeGetFreeVirtualMemory() );          
            
            maxRAMLines = (unsigned int)MAX( 1, freeMem /
              ((double)lineSize_) );
          }        
          
          // Allocating RAM lines
          
          unsigned int ramLinesNmb = MIN( maxRAMLines, totalLines_ );
          
          for( unsigned int allLinesPtrsVecIdx = 0 ; allLinesPtrsVecIdx <
            ramLinesNmb ; ++allLinesPtrsVecIdx )
          {
            allLinesPtrsVec_[ allLinesPtrsVecIdx ] = 
              new ElementType[ totalColumns_ ];
              
            if( allLinesPtrsVec_[ allLinesPtrsVecIdx ] == 0 )
            {
              clear();
            
              TEAGN_LOG_AND_RETURN( "Tiles allocation error" )
            }
            else
            {
              ramLinesIndexesVec_.push_back( allLinesPtrsVecIdx );
            }
          }
          
          // Allocating Disk lines
        
          if( ! allocateDiskLines( ramLinesNmb ) )
          {
            clear();
            
            TEAGN_LOG_AND_RETURN( "Tiles allocation error" )
          }          
        }
      }
      catch(...)
      {
        clear();
        
        TEAGN_LOG_AND_RETURN( "Tiles allocation error" )
      } 
    }
    
    return true;
  }
  
  template< class ElementType >
  void TePDIMatrix< ElementType >::clear()
  {
    const unsigned int allLinesPtrsVecSize = (unsigned int)
      allLinesPtrsVec_.size();
      
    for( unsigned int allLinesPtrsVecIdx = 0 ; allLinesPtrsVecIdx < 
      allLinesPtrsVecSize ; ++allLinesPtrsVecIdx ) 
    {
      if( allLinesPtrsVec_[ allLinesPtrsVecIdx ] ) 
      {
        delete[] allLinesPtrsVec_[ allLinesPtrsVecIdx ];
      }
    }  
    allLinesPtrsVec_.clear();
    
    for( unsigned int openDiskFilesVecIdx = 0 ; openDiskFilesVecIdx < 
      openDiskFilesVec_.size() ; ++openDiskFilesVecIdx )
    {
      TeTempFilesRemover::instance().removeFile( openDiskFilesVec_[ 
        openDiskFilesVecIdx ].second );      
    } 
    openDiskFilesVec_.clear(); 
       
    diskLinesVec_.clear();
    
    ramLinesIndexesVec_.clear();
    
    if( swapLinePtr_ ) delete swapLinePtr_;
  
    init();
  }    
  
  template< class ElementType >
  unsigned int TePDIMatrix< ElementType >::GetLines() const
  {
    return totalLines_;
  }

  
  template< class ElementType >
  unsigned int TePDIMatrix< ElementType >::GetColumns() const
  {
    return totalColumns_;
  }
  
  
  template< class ElementType >
  bool TePDIMatrix< ElementType >::IsEmpty() const
  {
    return ( totalLines_ == 0 ) ? true : false;
  }
  

  template< class ElementType >
  const TePDIMatrix< ElementType >& TePDIMatrix< ElementType >::operator=(
    const TePDIMatrix< ElementType >& external )
  {
    TEAGN_TRUE_OR_THROW( 
      Reset( external.totalLines_, external.totalColumns_,
      memoryPolicy_, maxTmpFileSize_, maxMemPercentUsage_ ),
      "Unable to initiate the matrix object" );
    
    unsigned int column = 0;;
    ElementType const* inLinePtr = 0;
    ElementType* outLinePtr = 0;
    
    for( unsigned int line = 0 ; line < totalLines_ ; ++line ) 
    {
      inLinePtr = external.getScanLine( line );
      outLinePtr = getScanLine( line );
      
      for( column = 0 ; column < totalColumns_ ; ++column ) {
        outLinePtr[ column ] = inLinePtr[ column ];
      }
    }

    return *this;
  }
  
  template< class ElementType >
  typename TePDIMatrix< ElementType >::MemoryPolicy TePDIMatrix< ElementType >::getMemPolicy() const
  {
    return memoryPolicy_;
  }  
  
  template< class ElementType >
  unsigned long int TePDIMatrix< ElementType >::getMaxTmpFileSize() const
  {
    return maxTmpFileSize_;
  }    
  
  template< class ElementType >
  bool TePDIMatrix< ElementType >::allocateDiskLines( unsigned int startingLineIdx )
  {
    const unsigned long int diskLinesNmb = totalLines_ - startingLineIdx;
    
    if( diskLinesNmb )
    {    
      const unsigned long int maxLinesPerFile = ( unsigned long int )
        floor( ( (double)maxTmpFileSize_ ) / ( (double) lineSize_ ) );
          
      const unsigned long int maxFileSize = (unsigned long int)
        ( maxLinesPerFile * lineSize_ );
        
      const unsigned int filesNumber = (unsigned int)ceil( ((double)diskLinesNmb) 
        / ((double)maxLinesPerFile) );

      // Resizing the disk lines info vector
      
      diskLinesVec_.resize( totalLines_ );
      
      // Allocating each file
      
      unsigned int remainingLinesNmb = diskLinesNmb;
      unsigned int fileSize = 0;
      unsigned int fileLinesNumber = 0;    
      std::pair< FILE*, std::string > auxFileData;  
      unsigned int diskLinesVecIdx = startingLineIdx;
      
      for( unsigned int fileIdx = 0 ; fileIdx < filesNumber ; ++fileIdx )
      {
        // Defining the current file size
        
        fileSize = maxFileSize;
        fileLinesNumber = maxLinesPerFile;
        
        if( remainingLinesNmb < maxLinesPerFile )
        {
          fileSize = (unsigned long int)( lineSize_ * remainingLinesNmb );
          fileLinesNumber = remainingLinesNmb;
        }
        
        remainingLinesNmb -= fileLinesNumber;
        
        // allocating the file 
        
        TEAGN_TRUE_OR_RETURN( TeGetTempFileName( auxFileData.second ),
          "Unable to get temporary file name" );        
        
        if( ! createNewDiskFile( fileSize, auxFileData.second, 
          &( auxFileData.first) ) )
        {
          TEAGN_LOGERR( "Unable to create temporary disk file" );
          
          return false;           
        }
        else
        {
          openDiskFilesVec_.push_back( auxFileData );
          
          TeTempFilesRemover::instance().addFile( auxFileData.second,
            auxFileData.first );    
                
          for( unsigned int lineIdx = 0; lineIdx < fileLinesNumber ; ++lineIdx )
          {
            diskLinesVec_[ diskLinesVecIdx ].filePtr_ = auxFileData.first;
            diskLinesVec_[ diskLinesVecIdx ].fileOff_ = lineIdx * lineSize_;
            
            ++diskLinesVecIdx;
          }
        }
      }
    }
    
    return true;
  }
  
  template< class ElementType >
  ElementType* TePDIMatrix< ElementType >::getScanLine( const unsigned int& line ) const
  {
    TEAGN_DEBUG_CONDITION( line < allLinesPtrsVec_.size(),
      "Invalid tile index" );
      
    if( allLinesPtrsVec_[ line ] ) 
    {
      return allLinesPtrsVec_[ line ];
    } 
    else 
    {
      // Finding the swap line index 

      TEAGN_DEBUG_CONDITION( nextSwapLineRamLinesIndexesVecIdx_ <
        ramLinesIndexesVec_.size(), "Internal error" );
      getScanLine_swapLineIdx_ = ramLinesIndexesVec_[ 
        nextSwapLineRamLinesIndexesVecIdx_ ];
      
      TEAGN_DEBUG_CONDITION( line < diskLinesVec_.size(), "Internal error" );
      DiskLinesInfoNode& inLineData = diskLinesVec_[ line ];
      
      TEAGN_DEBUG_CONDITION( getScanLine_swapLineIdx_ < 
        diskLinesVec_.size(), "Internal error" )
      DiskLinesInfoNode& outLineData = diskLinesVec_[ 
        getScanLine_swapLineIdx_ ];
        
      /* Reading the required tile into RAM (swapTilePtr_) */
      
      TEAGN_DEBUG_CONDITION( inLineData.filePtr_, "Internal error" );
      TEAGN_TRUE_OR_THROW( 0 == fseek( inLineData.filePtr_, 
        (long)( inLineData.fileOff_ ), SEEK_SET ),
        "File seek error" )
        
      TEAGN_DEBUG_CONDITION( swapLinePtr_, "Internal error" );
      TEAGN_TRUE_OR_THROW( 1 == fread( (void*)swapLinePtr_, 
        (size_t)( lineSize_ ), 1, inLineData.filePtr_ ),
        "File read error" )
    
      /* Flushing the swap tile to disk */
        
      TEAGN_TRUE_OR_THROW( 0 == fseek( inLineData.filePtr_, 
        (long)( inLineData.fileOff_ ), SEEK_SET ),
        "File seek error" );
        
      TEAGN_DEBUG_CONDITION( getScanLine_swapLineIdx_ < 
        allLinesPtrsVec_.size(), "Internal error" );          
      TEAGN_TRUE_OR_THROW( 1 == fwrite( (void*)allLinesPtrsVec_[ 
        getScanLine_swapLineIdx_ ], 
        (size_t)( lineSize_ ), 1, inLineData.filePtr_ ),
        "File write error" )        
      
      // Updating the tile pointers
      
      getScanLine_linePtr_ = allLinesPtrsVec_[ getScanLine_swapLineIdx_ ];
      
      allLinesPtrsVec_[ getScanLine_swapLineIdx_ ] = 0;
      
      allLinesPtrsVec_[ line ] = swapLinePtr_;
      
      swapLinePtr_ = getScanLine_linePtr_;
      
      /* Updating the info vectors */
          
      outLineData.filePtr_ = inLineData.filePtr_;
      outLineData.fileOff_ = inLineData.fileOff_;
      
      inLineData.filePtr_ = 0;
      inLineData.fileOff_ = 0;    
      
      ramLinesIndexesVec_[ nextSwapLineRamLinesIndexesVecIdx_ ] =
        line;
      nextSwapLineRamLinesIndexesVecIdx_ = 
        ( ( nextSwapLineRamLinesIndexesVecIdx_ + 1 ) % 
        ((unsigned int)ramLinesIndexesVec_.size()) );
      TEAGN_DEBUG_CONDITION( nextSwapLineRamLinesIndexesVecIdx_ <
        ramLinesIndexesVec_.size(), "Internal error" );          
          
      return allLinesPtrsVec_[ line ];
    }
  } 
  
  template< class ElementType >
  bool TePDIMatrix< ElementType >::createNewDiskFile( unsigned long int size,
    const std::string& filename, FILE** fileptr ) const
  {
    TEAGN_TRUE_OR_RETURN( ! filename.empty(),
      "Invalid file name" );
      
    (*fileptr) = fopen( filename.c_str(), "wb+" );
    TEAGN_TRUE_OR_RETURN( (*fileptr) != 0, "Invalid file pointer" )
    
    long seekoff = (long)( size - 1 );
    
    if( 0 != fseek( (*fileptr), seekoff, SEEK_SET ) )
    {
      fclose( (*fileptr) );
      TEAGN_LOGERR( "File seek error" );
      return false;
    }
  
    unsigned char c = '\0';
    if( 1 != fwrite( &c, 1, 1, (*fileptr) ) )
    {
      fclose( (*fileptr) );
      TEAGN_LOGERR( "File write error" );
      return false;
    }
      
    return true;
  }  
  
#endif

