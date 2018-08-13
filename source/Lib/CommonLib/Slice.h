/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2018, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     Slice.h
    \brief    slice header and SPS class (header)
*/

#ifndef __SLICE__
#define __SLICE__

#include <cstring>
#include <list>
#include <map>
#include <vector>
#include "CommonDef.h"
#include "Rom.h"
#include "ChromaFormat.h"
#include "Common.h"

//! \ingroup CommonLib
//! \{



struct Picture;
class Pic;
class TrQuant;
// ====================================================================================================================
// Constants
// ====================================================================================================================
class PreCalcValues;
static const UInt REF_PIC_LIST_NUM_IDX=32;

typedef std::list<Picture*> PicList;

// ====================================================================================================================
// Class definition
// ====================================================================================================================

/// Reference Picture Set class
class ReferencePictureSet
{
private:
  Int  m_numberOfPictures;
  Int  m_numberOfNegativePictures;
  Int  m_numberOfPositivePictures;
  Int  m_numberOfLongtermPictures;
  Int  m_deltaPOC[MAX_NUM_REF_PICS];
  Int  m_POC[MAX_NUM_REF_PICS];
  bool m_used[MAX_NUM_REF_PICS];
  bool m_interRPSPrediction;
  Int  m_deltaRIdxMinus1;
  Int  m_deltaRPS;
  Int  m_numRefIdc;
  Int  m_refIdc[MAX_NUM_REF_PICS+1];
  bool m_bCheckLTMSB[MAX_NUM_REF_PICS];
  Int  m_pocLSBLT[MAX_NUM_REF_PICS];
  Int  m_deltaPOCMSBCycleLT[MAX_NUM_REF_PICS];
  bool m_deltaPocMSBPresentFlag[MAX_NUM_REF_PICS];

public:
          ReferencePictureSet();
  virtual ~ReferencePictureSet();
  Int     getPocLSBLT(Int i) const                     { return m_pocLSBLT[i];               }
  void    setPocLSBLT(Int i, Int x)                    { m_pocLSBLT[i] = x;                  }
  Int     getDeltaPocMSBCycleLT(Int i) const           { return m_deltaPOCMSBCycleLT[i];     }
  void    setDeltaPocMSBCycleLT(Int i, Int x)          { m_deltaPOCMSBCycleLT[i] = x;        }
  bool    getDeltaPocMSBPresentFlag(Int i) const       { return m_deltaPocMSBPresentFlag[i]; }
  void    setDeltaPocMSBPresentFlag(Int i, bool x)     { m_deltaPocMSBPresentFlag[i] = x;    }
  void    setUsed(Int bufferNum, bool used);
  void    setDeltaPOC(Int bufferNum, Int deltaPOC);
  void    setPOC(Int bufferNum, Int deltaPOC);
  void    setNumberOfPictures(Int numberOfPictures);
  void    setCheckLTMSBPresent(Int bufferNum, bool b );
  bool    getCheckLTMSBPresent(Int bufferNum) const;

  Int     getUsed(Int bufferNum) const;
  Int     getDeltaPOC(Int bufferNum) const;
  Int     getPOC(Int bufferNum) const;
  Int     getNumberOfPictures() const;

  void    setNumberOfNegativePictures(Int number)      { m_numberOfNegativePictures = number; }
  Int     getNumberOfNegativePictures() const          { return m_numberOfNegativePictures;   }
  void    setNumberOfPositivePictures(Int number)      { m_numberOfPositivePictures = number; }
  Int     getNumberOfPositivePictures() const          { return m_numberOfPositivePictures;   }
  void    setNumberOfLongtermPictures(Int number)      { m_numberOfLongtermPictures = number; }
  Int     getNumberOfLongtermPictures() const          { return m_numberOfLongtermPictures;   }

  void    setInterRPSPrediction(bool flag)             { m_interRPSPrediction = flag;         }
  bool    getInterRPSPrediction() const                { return m_interRPSPrediction;         }
  void    setDeltaRIdxMinus1(Int x)                    { m_deltaRIdxMinus1 = x;               }
  Int     getDeltaRIdxMinus1() const                   { return m_deltaRIdxMinus1;            }
  void    setDeltaRPS(Int x)                           { m_deltaRPS = x;                      }
  Int     getDeltaRPS() const                          { return m_deltaRPS;                   }
  void    setNumRefIdc(Int x)                          { m_numRefIdc = x;                     }
  Int     getNumRefIdc() const                         { return m_numRefIdc;                  }

  void    setRefIdc(Int bufferNum, Int refIdc);
  Int     getRefIdc(Int bufferNum) const ;

  void    sortDeltaPOC();
  void    printDeltaPOC() const;
};

/// Reference Picture Set set class
class RPSList
{
private:
  std::vector<ReferencePictureSet> m_referencePictureSets;

public:
                                 RPSList()                                            { }
  virtual                        ~RPSList()                                           { }

  void                           create  (Int numberOfEntries)                            { m_referencePictureSets.resize(numberOfEntries);         }
  void                           destroy ()                                               { }


  ReferencePictureSet*       getReferencePictureSet(Int referencePictureSetNum)       { return &m_referencePictureSets[referencePictureSetNum]; }
  const ReferencePictureSet* getReferencePictureSet(Int referencePictureSetNum) const { return &m_referencePictureSets[referencePictureSetNum]; }

  Int                            getNumberOfReferencePictureSets() const                  { return Int(m_referencePictureSets.size());              }
};

#if HEVC_USE_SCALING_LISTS
/// SCALING_LIST class
class ScalingList
{
public:
             ScalingList();
  virtual    ~ScalingList()                                                 { }
  Int*       getScalingListAddress(UInt sizeId, UInt listId)                    { return &(m_scalingListCoef[sizeId][listId][0]);            } //!< get matrix coefficient
  const Int* getScalingListAddress(UInt sizeId, UInt listId) const              { return &(m_scalingListCoef[sizeId][listId][0]);            } //!< get matrix coefficient
  void       checkPredMode(UInt sizeId, UInt listId);

  void       setRefMatrixId(UInt sizeId, UInt listId, UInt u)                   { m_refMatrixId[sizeId][listId] = u;                         } //!< set reference matrix ID
  UInt       getRefMatrixId(UInt sizeId, UInt listId) const                     { return m_refMatrixId[sizeId][listId];                      } //!< get reference matrix ID

  const Int* getScalingListDefaultAddress(UInt sizeId, UInt listId);                                                                           //!< get default matrix coefficient
  void       processDefaultMatrix(UInt sizeId, UInt listId);

  void       setScalingListDC(UInt sizeId, UInt listId, UInt u)                 { m_scalingListDC[sizeId][listId] = u;                       } //!< set DC value
  Int        getScalingListDC(UInt sizeId, UInt listId) const                   { return m_scalingListDC[sizeId][listId];                    } //!< get DC value

  void       setScalingListPredModeFlag(UInt sizeId, UInt listId, bool bIsDPCM) { m_scalingListPredModeFlagIsDPCM[sizeId][listId] = bIsDPCM; }
  bool       getScalingListPredModeFlag(UInt sizeId, UInt listId) const         { return m_scalingListPredModeFlagIsDPCM[sizeId][listId];    }

  void       checkDcOfMatrix();
  void       processRefMatrix(UInt sizeId, UInt listId , UInt refListId );
  bool       xParseScalingList(const std::string &fileName);
  void       setDefaultScalingList();
  bool       checkDefaultScalingList();

private:
  void       outputScalingLists(std::ostream &os) const;
  bool             m_scalingListPredModeFlagIsDPCM [SCALING_LIST_SIZE_NUM][SCALING_LIST_NUM]; //!< reference list index
  Int              m_scalingListDC                 [SCALING_LIST_SIZE_NUM][SCALING_LIST_NUM]; //!< the DC value of the matrix coefficient for 16x16
  UInt             m_refMatrixId                   [SCALING_LIST_SIZE_NUM][SCALING_LIST_NUM]; //!< RefMatrixID
  std::vector<Int> m_scalingListCoef               [SCALING_LIST_SIZE_NUM][SCALING_LIST_NUM]; //!< quantization matrix
};
#endif

class ProfileTierLevel
{
  Int               m_profileSpace;
  Level::Tier       m_tierFlag;
  Profile::Name     m_profileIdc;
  bool              m_profileCompatibilityFlag[32];
  Level::Name       m_levelIdc;

  bool              m_progressiveSourceFlag;
  bool              m_interlacedSourceFlag;
  bool              m_nonPackedConstraintFlag;
  bool              m_frameOnlyConstraintFlag;
  UInt              m_bitDepthConstraintValue;
  ChromaFormat      m_chromaFormatConstraintValue;
  bool              m_intraConstraintFlag;
  bool              m_onePictureOnlyConstraintFlag;
  bool              m_lowerBitRateConstraintFlag;

public:
                ProfileTierLevel();

  Int           getProfileSpace() const                     { return m_profileSpace;                }
  void          setProfileSpace(Int x)                      { m_profileSpace = x;                   }

  Level::Tier   getTierFlag() const                         { return m_tierFlag;                    }
  void          setTierFlag(Level::Tier x)                  { m_tierFlag = x;                       }

  Profile::Name getProfileIdc() const                       { return m_profileIdc;                  }
  void          setProfileIdc(Profile::Name x)              { m_profileIdc = x;                     }

  bool          getProfileCompatibilityFlag(Int i) const    { return m_profileCompatibilityFlag[i]; }
  void          setProfileCompatibilityFlag(Int i, bool x)  { m_profileCompatibilityFlag[i] = x;    }

  Level::Name   getLevelIdc() const                         { return m_levelIdc;                    }
  void          setLevelIdc(Level::Name x)                  { m_levelIdc = x;                       }

  bool          getProgressiveSourceFlag() const            { return m_progressiveSourceFlag;       }
  void          setProgressiveSourceFlag(bool b)            { m_progressiveSourceFlag = b;          }

  bool          getInterlacedSourceFlag() const             { return m_interlacedSourceFlag;        }
  void          setInterlacedSourceFlag(bool b)             { m_interlacedSourceFlag = b;           }

  bool          getNonPackedConstraintFlag() const          { return m_nonPackedConstraintFlag;     }
  void          setNonPackedConstraintFlag(bool b)          { m_nonPackedConstraintFlag = b;        }

  bool          getFrameOnlyConstraintFlag() const          { return m_frameOnlyConstraintFlag;     }
  void          setFrameOnlyConstraintFlag(bool b)          { m_frameOnlyConstraintFlag = b;        }

  UInt          getBitDepthConstraint() const               { return m_bitDepthConstraintValue;     }
  void          setBitDepthConstraint(UInt bitDepth)        { m_bitDepthConstraintValue=bitDepth;   }

  ChromaFormat  getChromaFormatConstraint() const           { return m_chromaFormatConstraintValue; }
  void          setChromaFormatConstraint(ChromaFormat fmt) { m_chromaFormatConstraintValue=fmt;    }

  bool          getIntraConstraintFlag() const              { return m_intraConstraintFlag;         }
  void          setIntraConstraintFlag(bool b)              { m_intraConstraintFlag = b;            }

  bool          getOnePictureOnlyConstraintFlag() const     { return m_onePictureOnlyConstraintFlag;}
  void          setOnePictureOnlyConstraintFlag(bool b)     { m_onePictureOnlyConstraintFlag = b;   }

  bool          getLowerBitRateConstraintFlag() const       { return m_lowerBitRateConstraintFlag;  }
  void          setLowerBitRateConstraintFlag(bool b)       { m_lowerBitRateConstraintFlag = b;     }
};


class PTL
{
  ProfileTierLevel m_generalPTL;
  ProfileTierLevel m_subLayerPTL    [MAX_TLAYER-1];      // max. value of max_sub_layers_minus1 is MAX_TLAYER-1 (= 6)
  bool m_subLayerProfilePresentFlag [MAX_TLAYER-1];
  bool m_subLayerLevelPresentFlag   [MAX_TLAYER-1];

public:
                          PTL();
  bool                    getSubLayerProfilePresentFlag(Int i) const   { return m_subLayerProfilePresentFlag[i]; }
  void                    setSubLayerProfilePresentFlag(Int i, bool x) { m_subLayerProfilePresentFlag[i] = x;    }

  bool                    getSubLayerLevelPresentFlag(Int i) const     { return m_subLayerLevelPresentFlag[i];   }
  void                    setSubLayerLevelPresentFlag(Int i, bool x)   { m_subLayerLevelPresentFlag[i] = x;      }

  ProfileTierLevel*       getGeneralPTL()                              { return &m_generalPTL;                   }
  const ProfileTierLevel* getGeneralPTL() const                        { return &m_generalPTL;                   }
  ProfileTierLevel*       getSubLayerPTL(Int i)                        { return &m_subLayerPTL[i];               }
  const ProfileTierLevel* getSubLayerPTL(Int i) const                  { return &m_subLayerPTL[i];               }
};

struct HrdSubLayerInfo
{
  bool fixedPicRateFlag;
  bool fixedPicRateWithinCvsFlag;
  UInt picDurationInTcMinus1;
  bool lowDelayHrdFlag;
  UInt cpbCntMinus1;
  UInt bitRateValueMinus1[MAX_CPB_CNT][2];
  UInt cpbSizeValue      [MAX_CPB_CNT][2];
  UInt ducpbSizeValue    [MAX_CPB_CNT][2];
  bool cbrFlag           [MAX_CPB_CNT][2];
  UInt duBitRateValue    [MAX_CPB_CNT][2];
};

class HRD
{
private:
  bool m_nalHrdParametersPresentFlag;
  bool m_vclHrdParametersPresentFlag;
  bool m_subPicCpbParamsPresentFlag;
  UInt m_tickDivisorMinus2;
  UInt m_duCpbRemovalDelayLengthMinus1;
  bool m_subPicCpbParamsInPicTimingSEIFlag;
  UInt m_dpbOutputDelayDuLengthMinus1;
  UInt m_bitRateScale;
  UInt m_cpbSizeScale;
  UInt m_ducpbSizeScale;
  UInt m_initialCpbRemovalDelayLengthMinus1;
  UInt m_cpbRemovalDelayLengthMinus1;
  UInt m_dpbOutputDelayLengthMinus1;
  HrdSubLayerInfo m_HRD[MAX_TLAYER];

public:
  HRD()
  :m_nalHrdParametersPresentFlag       (0)
  ,m_vclHrdParametersPresentFlag       (0)
  ,m_subPicCpbParamsPresentFlag        (false)
  ,m_tickDivisorMinus2                 (0)
  ,m_duCpbRemovalDelayLengthMinus1     (0)
  ,m_subPicCpbParamsInPicTimingSEIFlag (false)
  ,m_dpbOutputDelayDuLengthMinus1      (0)
  ,m_bitRateScale                      (0)
  ,m_cpbSizeScale                      (0)
  ,m_initialCpbRemovalDelayLengthMinus1(23)
  ,m_cpbRemovalDelayLengthMinus1       (23)
  ,m_dpbOutputDelayLengthMinus1        (23)
  {}

  virtual ~HRD() {}

  void    setNalHrdParametersPresentFlag( bool flag )                                { m_nalHrdParametersPresentFlag = flag;                      }
  bool    getNalHrdParametersPresentFlag( ) const                                    { return m_nalHrdParametersPresentFlag;                      }

  void    setVclHrdParametersPresentFlag( bool flag )                                { m_vclHrdParametersPresentFlag = flag;                      }
  bool    getVclHrdParametersPresentFlag( ) const                                    { return m_vclHrdParametersPresentFlag;                      }

  void    setSubPicCpbParamsPresentFlag( bool flag )                                 { m_subPicCpbParamsPresentFlag = flag;                       }
  bool    getSubPicCpbParamsPresentFlag( ) const                                     { return m_subPicCpbParamsPresentFlag;                       }

  void    setTickDivisorMinus2( UInt value )                                         { m_tickDivisorMinus2 = value;                               }
  UInt    getTickDivisorMinus2( ) const                                              { return m_tickDivisorMinus2;                                }

  void    setDuCpbRemovalDelayLengthMinus1( UInt value )                             { m_duCpbRemovalDelayLengthMinus1 = value;                   }
  UInt    getDuCpbRemovalDelayLengthMinus1( ) const                                  { return m_duCpbRemovalDelayLengthMinus1;                    }

  void    setSubPicCpbParamsInPicTimingSEIFlag( bool flag)                           { m_subPicCpbParamsInPicTimingSEIFlag = flag;                }
  bool    getSubPicCpbParamsInPicTimingSEIFlag( ) const                              { return m_subPicCpbParamsInPicTimingSEIFlag;                }

  void    setDpbOutputDelayDuLengthMinus1(UInt value )                               { m_dpbOutputDelayDuLengthMinus1 = value;                    }
  UInt    getDpbOutputDelayDuLengthMinus1( ) const                                   { return m_dpbOutputDelayDuLengthMinus1;                     }

  void    setBitRateScale( UInt value )                                              { m_bitRateScale = value;                                    }
  UInt    getBitRateScale( ) const                                                   { return m_bitRateScale;                                     }

  void    setCpbSizeScale( UInt value )                                              { m_cpbSizeScale = value;                                    }
  UInt    getCpbSizeScale( ) const                                                   { return m_cpbSizeScale;                                     }
  void    setDuCpbSizeScale( UInt value )                                            { m_ducpbSizeScale = value;                                  }
  UInt    getDuCpbSizeScale( ) const                                                 { return m_ducpbSizeScale;                                   }

  void    setInitialCpbRemovalDelayLengthMinus1( UInt value )                        { m_initialCpbRemovalDelayLengthMinus1 = value;              }
  UInt    getInitialCpbRemovalDelayLengthMinus1( ) const                             { return m_initialCpbRemovalDelayLengthMinus1;               }

  void    setCpbRemovalDelayLengthMinus1( UInt value )                               { m_cpbRemovalDelayLengthMinus1 = value;                     }
  UInt    getCpbRemovalDelayLengthMinus1( ) const                                    { return m_cpbRemovalDelayLengthMinus1;                      }

  void    setDpbOutputDelayLengthMinus1( UInt value )                                { m_dpbOutputDelayLengthMinus1 = value;                      }
  UInt    getDpbOutputDelayLengthMinus1( ) const                                     { return m_dpbOutputDelayLengthMinus1;                       }

  void    setFixedPicRateFlag( Int layer, bool flag )                                { m_HRD[layer].fixedPicRateFlag = flag;                      }
  bool    getFixedPicRateFlag( Int layer ) const                                     { return m_HRD[layer].fixedPicRateFlag;                      }

  void    setFixedPicRateWithinCvsFlag( Int layer, bool flag )                       { m_HRD[layer].fixedPicRateWithinCvsFlag = flag;             }
  bool    getFixedPicRateWithinCvsFlag( Int layer ) const                            { return m_HRD[layer].fixedPicRateWithinCvsFlag;             }

  void    setPicDurationInTcMinus1( Int layer, UInt value )                          { m_HRD[layer].picDurationInTcMinus1 = value;                }
  UInt    getPicDurationInTcMinus1( Int layer ) const                                { return m_HRD[layer].picDurationInTcMinus1;                 }

  void    setLowDelayHrdFlag( Int layer, bool flag )                                 { m_HRD[layer].lowDelayHrdFlag = flag;                       }
  bool    getLowDelayHrdFlag( Int layer ) const                                      { return m_HRD[layer].lowDelayHrdFlag;                       }

  void    setCpbCntMinus1( Int layer, UInt value )                                   { m_HRD[layer].cpbCntMinus1 = value;                         }
  UInt    getCpbCntMinus1( Int layer ) const                                         { return m_HRD[layer].cpbCntMinus1;                          }

  void    setBitRateValueMinus1( Int layer, Int cpbcnt, Int nalOrVcl, UInt value )   { m_HRD[layer].bitRateValueMinus1[cpbcnt][nalOrVcl] = value; }
  UInt    getBitRateValueMinus1( Int layer, Int cpbcnt, Int nalOrVcl ) const         { return m_HRD[layer].bitRateValueMinus1[cpbcnt][nalOrVcl];  }

  void    setCpbSizeValueMinus1( Int layer, Int cpbcnt, Int nalOrVcl, UInt value )   { m_HRD[layer].cpbSizeValue[cpbcnt][nalOrVcl] = value;       }
  UInt    getCpbSizeValueMinus1( Int layer, Int cpbcnt, Int nalOrVcl ) const         { return m_HRD[layer].cpbSizeValue[cpbcnt][nalOrVcl];        }
  void    setDuCpbSizeValueMinus1( Int layer, Int cpbcnt, Int nalOrVcl, UInt value ) { m_HRD[layer].ducpbSizeValue[cpbcnt][nalOrVcl] = value;     }
  UInt    getDuCpbSizeValueMinus1( Int layer, Int cpbcnt, Int nalOrVcl ) const       { return m_HRD[layer].ducpbSizeValue[cpbcnt][nalOrVcl];      }
  void    setDuBitRateValueMinus1( Int layer, Int cpbcnt, Int nalOrVcl, UInt value ) { m_HRD[layer].duBitRateValue[cpbcnt][nalOrVcl] = value;     }
  UInt    getDuBitRateValueMinus1(Int layer, Int cpbcnt, Int nalOrVcl ) const        { return m_HRD[layer].duBitRateValue[cpbcnt][nalOrVcl];      }
  void    setCbrFlag( Int layer, Int cpbcnt, Int nalOrVcl, bool value )              { m_HRD[layer].cbrFlag[cpbcnt][nalOrVcl] = value;            }
  bool    getCbrFlag( Int layer, Int cpbcnt, Int nalOrVcl ) const                    { return m_HRD[layer].cbrFlag[cpbcnt][nalOrVcl];             }

  bool    getCpbDpbDelaysPresentFlag( ) const                      { return getNalHrdParametersPresentFlag() || getVclHrdParametersPresentFlag(); }
};

class TimingInfo
{
  bool m_timingInfoPresentFlag;
  UInt m_numUnitsInTick;
  UInt m_timeScale;
  bool m_pocProportionalToTimingFlag;
  Int  m_numTicksPocDiffOneMinus1;
public:
  TimingInfo()
  : m_timingInfoPresentFlag      (false)
  , m_numUnitsInTick             (1001)
  , m_timeScale                  (60000)
  , m_pocProportionalToTimingFlag(false)
  , m_numTicksPocDiffOneMinus1   (0)
  {}

  void setTimingInfoPresentFlag( bool flag )   { m_timingInfoPresentFlag = flag;       }
  bool getTimingInfoPresentFlag( ) const       { return m_timingInfoPresentFlag;       }

  void setNumUnitsInTick( UInt value )         { m_numUnitsInTick = value;             }
  UInt getNumUnitsInTick( ) const              { return m_numUnitsInTick;              }

  void setTimeScale( UInt value )              { m_timeScale = value;                  }
  UInt getTimeScale( ) const                   { return m_timeScale;                   }

  void setPocProportionalToTimingFlag(bool x)  { m_pocProportionalToTimingFlag = x;    }
  bool getPocProportionalToTimingFlag( ) const { return m_pocProportionalToTimingFlag; }

  void setNumTicksPocDiffOneMinus1(Int x)      { m_numTicksPocDiffOneMinus1 = x;       }
  Int  getNumTicksPocDiffOneMinus1( ) const    { return m_numTicksPocDiffOneMinus1;    }
};

struct ChromaQpAdj
{
  union
  {
    struct {
      Int CbOffset;
      Int CrOffset;
    } comp;
    Int offset[2]; /* two chroma components */
  } u;
};

#if HEVC_VPS
class VPS
{
private:
  Int                   m_VPSId;
  UInt                  m_uiMaxTLayers;
  UInt                  m_uiMaxLayers;
  bool                  m_bTemporalIdNestingFlag;

  UInt                  m_numReorderPics[MAX_TLAYER];
  UInt                  m_uiMaxDecPicBuffering[MAX_TLAYER];
  UInt                  m_uiMaxLatencyIncrease[MAX_TLAYER]; // Really max latency increase plus 1 (value 0 expresses no limit)

  UInt                  m_numHrdParameters;
  UInt                  m_maxNuhReservedZeroLayerId;
  std::vector<HRD>      m_hrdParameters;
  std::vector<UInt>     m_hrdOpSetIdx;
  std::vector<bool>     m_cprmsPresentFlag;
  UInt                  m_numOpSets;
  bool                  m_layerIdIncludedFlag[MAX_VPS_OP_SETS_PLUS1][MAX_VPS_NUH_RESERVED_ZERO_LAYER_ID_PLUS1];

  PTL                   m_pcPTL;
  TimingInfo            m_timingInfo;

public:
                    VPS();

  virtual           ~VPS();

  void              createHrdParamBuffer()
  {
    m_hrdParameters   .resize(getNumHrdParameters());
    m_hrdOpSetIdx     .resize(getNumHrdParameters());
    m_cprmsPresentFlag.resize(getNumHrdParameters());
  }

  HRD*              getHrdParameters( UInt i )                           { return &m_hrdParameters[ i ];                                    }
  const HRD*        getHrdParameters( UInt i ) const                     { return &m_hrdParameters[ i ];                                    }
  UInt              getHrdOpSetIdx( UInt i ) const                       { return m_hrdOpSetIdx[ i ];                                       }
  void              setHrdOpSetIdx( UInt val, UInt i )                   { m_hrdOpSetIdx[ i ] = val;                                        }
  bool              getCprmsPresentFlag( UInt i ) const                  { return m_cprmsPresentFlag[ i ];                                  }
  void              setCprmsPresentFlag( bool val, UInt i )              { m_cprmsPresentFlag[ i ] = val;                                   }

  Int               getVPSId() const                                     { return m_VPSId;                                                  }
  void              setVPSId(Int i)                                      { m_VPSId = i;                                                     }

  UInt              getMaxTLayers() const                                { return m_uiMaxTLayers;                                           }
  void              setMaxTLayers(UInt t)                                { m_uiMaxTLayers = t;                                              }

  UInt              getMaxLayers() const                                 { return m_uiMaxLayers;                                            }
  void              setMaxLayers(UInt l)                                 { m_uiMaxLayers = l;                                               }

  bool              getTemporalNestingFlag() const                       { return m_bTemporalIdNestingFlag;                                 }
  void              setTemporalNestingFlag(bool t)                       { m_bTemporalIdNestingFlag = t;                                    }

  void              setNumReorderPics(UInt v, UInt tLayer)               { m_numReorderPics[tLayer] = v;                                    }
  UInt              getNumReorderPics(UInt tLayer) const                 { return m_numReorderPics[tLayer];                                 }

  void              setMaxDecPicBuffering(UInt v, UInt tLayer)           { CHECK(tLayer >= MAX_TLAYER, "Invalid T-layer"); m_uiMaxDecPicBuffering[tLayer] = v; }
  UInt              getMaxDecPicBuffering(UInt tLayer) const             { return m_uiMaxDecPicBuffering[tLayer];                           }

  void              setMaxLatencyIncrease(UInt v, UInt tLayer)           { m_uiMaxLatencyIncrease[tLayer] = v;                              }
  UInt              getMaxLatencyIncrease(UInt tLayer) const             { return m_uiMaxLatencyIncrease[tLayer];                           }

  UInt              getNumHrdParameters() const                          { return m_numHrdParameters;                                       }
  void              setNumHrdParameters(UInt v)                          { m_numHrdParameters = v;                                          }

  UInt              getMaxNuhReservedZeroLayerId() const                 { return m_maxNuhReservedZeroLayerId;                              }
  void              setMaxNuhReservedZeroLayerId(UInt v)                 { m_maxNuhReservedZeroLayerId = v;                                 }

  UInt              getMaxOpSets() const                                 { return m_numOpSets;                                              }
  void              setMaxOpSets(UInt v)                                 { m_numOpSets = v;                                                 }
  bool              getLayerIdIncludedFlag(UInt opsIdx, UInt id) const   { return m_layerIdIncludedFlag[opsIdx][id];                        }
  void              setLayerIdIncludedFlag(bool v, UInt opsIdx, UInt id) { m_layerIdIncludedFlag[opsIdx][id] = v;                           }

  PTL*              getPTL()                                             { return &m_pcPTL;                                                 }
  const PTL*        getPTL() const                                       { return &m_pcPTL;                                                 }
  TimingInfo*       getTimingInfo()                                      { return &m_timingInfo;                                            }
  const TimingInfo* getTimingInfo() const                                { return &m_timingInfo;                                            }
};
#endif

class Window
{
private:
  bool m_enabledFlag;
  Int  m_winLeftOffset;
  Int  m_winRightOffset;
  Int  m_winTopOffset;
  Int  m_winBottomOffset;
public:
  Window()
  : m_enabledFlag    (false)
  , m_winLeftOffset  (0)
  , m_winRightOffset (0)
  , m_winTopOffset   (0)
  , m_winBottomOffset(0)
  { }

  bool getWindowEnabledFlag() const   { return m_enabledFlag;                          }
  Int  getWindowLeftOffset() const    { return m_enabledFlag ? m_winLeftOffset : 0;    }
  void setWindowLeftOffset(Int val)   { m_winLeftOffset = val; m_enabledFlag = true;   }
  Int  getWindowRightOffset() const   { return m_enabledFlag ? m_winRightOffset : 0;   }
  void setWindowRightOffset(Int val)  { m_winRightOffset = val; m_enabledFlag = true;  }
  Int  getWindowTopOffset() const     { return m_enabledFlag ? m_winTopOffset : 0;     }
  void setWindowTopOffset(Int val)    { m_winTopOffset = val; m_enabledFlag = true;    }
  Int  getWindowBottomOffset() const  { return m_enabledFlag ? m_winBottomOffset: 0;   }
  void setWindowBottomOffset(Int val) { m_winBottomOffset = val; m_enabledFlag = true; }

  void setWindow(Int offsetLeft, Int offsetLRight, Int offsetLTop, Int offsetLBottom)
  {
    m_enabledFlag     = true;
    m_winLeftOffset   = offsetLeft;
    m_winRightOffset  = offsetLRight;
    m_winTopOffset    = offsetLTop;
    m_winBottomOffset = offsetLBottom;
  }
};


class VUI
{
private:
  bool       m_aspectRatioInfoPresentFlag;
  Int        m_aspectRatioIdc;
  Int        m_sarWidth;
  Int        m_sarHeight;
  bool       m_overscanInfoPresentFlag;
  bool       m_overscanAppropriateFlag;
  bool       m_videoSignalTypePresentFlag;
  Int        m_videoFormat;
  bool       m_videoFullRangeFlag;
  bool       m_colourDescriptionPresentFlag;
  Int        m_colourPrimaries;
  Int        m_transferCharacteristics;
  Int        m_matrixCoefficients;
  bool       m_chromaLocInfoPresentFlag;
  Int        m_chromaSampleLocTypeTopField;
  Int        m_chromaSampleLocTypeBottomField;
  bool       m_neutralChromaIndicationFlag;
  bool       m_fieldSeqFlag;
  Window     m_defaultDisplayWindow;
  bool       m_frameFieldInfoPresentFlag;
  bool       m_hrdParametersPresentFlag;
  bool       m_bitstreamRestrictionFlag;
#if HEVC_TILES_WPP
  bool       m_tilesFixedStructureFlag;
#endif
  bool       m_motionVectorsOverPicBoundariesFlag;
  bool       m_restrictedRefPicListsFlag;
  Int        m_minSpatialSegmentationIdc;
  Int        m_maxBytesPerPicDenom;
  Int        m_maxBitsPerMinCuDenom;
  Int        m_log2MaxMvLengthHorizontal;
  Int        m_log2MaxMvLengthVertical;
  HRD    m_hrdParameters;
  TimingInfo m_timingInfo;

public:
  VUI()
    : m_aspectRatioInfoPresentFlag        (false) //TODO: This initialiser list contains magic numbers
    , m_aspectRatioIdc                    (0)
    , m_sarWidth                          (0)
    , m_sarHeight                         (0)
    , m_overscanInfoPresentFlag           (false)
    , m_overscanAppropriateFlag           (false)
    , m_videoSignalTypePresentFlag        (false)
    , m_videoFormat                       (5)
    , m_videoFullRangeFlag                (false)
    , m_colourDescriptionPresentFlag      (false)
    , m_colourPrimaries                   (2)
    , m_transferCharacteristics           (2)
    , m_matrixCoefficients                (2)
    , m_chromaLocInfoPresentFlag          (false)
    , m_chromaSampleLocTypeTopField       (0)
    , m_chromaSampleLocTypeBottomField    (0)
    , m_neutralChromaIndicationFlag       (false)
    , m_fieldSeqFlag                      (false)
    , m_frameFieldInfoPresentFlag         (false)
    , m_hrdParametersPresentFlag          (false)
    , m_bitstreamRestrictionFlag          (false)
#if HEVC_TILES_WPP
    , m_tilesFixedStructureFlag           (false)
#endif
    , m_motionVectorsOverPicBoundariesFlag(true)
    , m_restrictedRefPicListsFlag         (1)
    , m_minSpatialSegmentationIdc         (0)
    , m_maxBytesPerPicDenom               (2)
    , m_maxBitsPerMinCuDenom              (1)
    , m_log2MaxMvLengthHorizontal         (15)
    , m_log2MaxMvLengthVertical           (15)
  {}

  virtual           ~VUI() {}

  bool              getAspectRatioInfoPresentFlag() const                  { return m_aspectRatioInfoPresentFlag;           }
  void              setAspectRatioInfoPresentFlag(bool i)                  { m_aspectRatioInfoPresentFlag = i;              }

  Int               getAspectRatioIdc() const                              { return m_aspectRatioIdc;                       }
  void              setAspectRatioIdc(Int i)                               { m_aspectRatioIdc = i;                          }

  Int               getSarWidth() const                                    { return m_sarWidth;                             }
  void              setSarWidth(Int i)                                     { m_sarWidth = i;                                }

  Int               getSarHeight() const                                   { return m_sarHeight;                            }
  void              setSarHeight(Int i)                                    { m_sarHeight = i;                               }

  bool              getOverscanInfoPresentFlag() const                     { return m_overscanInfoPresentFlag;              }
  void              setOverscanInfoPresentFlag(bool i)                     { m_overscanInfoPresentFlag = i;                 }

  bool              getOverscanAppropriateFlag() const                     { return m_overscanAppropriateFlag;              }
  void              setOverscanAppropriateFlag(bool i)                     { m_overscanAppropriateFlag = i;                 }

  bool              getVideoSignalTypePresentFlag() const                  { return m_videoSignalTypePresentFlag;           }
  void              setVideoSignalTypePresentFlag(bool i)                  { m_videoSignalTypePresentFlag = i;              }

  Int               getVideoFormat() const                                 { return m_videoFormat;                          }
  void              setVideoFormat(Int i)                                  { m_videoFormat = i;                             }

  bool              getVideoFullRangeFlag() const                          { return m_videoFullRangeFlag;                   }
  void              setVideoFullRangeFlag(bool i)                          { m_videoFullRangeFlag = i;                      }

  bool              getColourDescriptionPresentFlag() const                { return m_colourDescriptionPresentFlag;         }
  void              setColourDescriptionPresentFlag(bool i)                { m_colourDescriptionPresentFlag = i;            }

  Int               getColourPrimaries() const                             { return m_colourPrimaries;                      }
  void              setColourPrimaries(Int i)                              { m_colourPrimaries = i;                         }

  Int               getTransferCharacteristics() const                     { return m_transferCharacteristics;              }
  void              setTransferCharacteristics(Int i)                      { m_transferCharacteristics = i;                 }

  Int               getMatrixCoefficients() const                          { return m_matrixCoefficients;                   }
  void              setMatrixCoefficients(Int i)                           { m_matrixCoefficients = i;                      }

  bool              getChromaLocInfoPresentFlag() const                    { return m_chromaLocInfoPresentFlag;             }
  void              setChromaLocInfoPresentFlag(bool i)                    { m_chromaLocInfoPresentFlag = i;                }

  Int               getChromaSampleLocTypeTopField() const                 { return m_chromaSampleLocTypeTopField;          }
  void              setChromaSampleLocTypeTopField(Int i)                  { m_chromaSampleLocTypeTopField = i;             }

  Int               getChromaSampleLocTypeBottomField() const              { return m_chromaSampleLocTypeBottomField;       }
  void              setChromaSampleLocTypeBottomField(Int i)               { m_chromaSampleLocTypeBottomField = i;          }

  bool              getNeutralChromaIndicationFlag() const                 { return m_neutralChromaIndicationFlag;          }
  void              setNeutralChromaIndicationFlag(bool i)                 { m_neutralChromaIndicationFlag = i;             }

  bool              getFieldSeqFlag() const                                { return m_fieldSeqFlag;                         }
  void              setFieldSeqFlag(bool i)                                { m_fieldSeqFlag = i;                            }

  bool              getFrameFieldInfoPresentFlag() const                   { return m_frameFieldInfoPresentFlag;            }
  void              setFrameFieldInfoPresentFlag(bool i)                   { m_frameFieldInfoPresentFlag = i;               }

  Window&           getDefaultDisplayWindow()                              { return m_defaultDisplayWindow;                 }
  const Window&     getDefaultDisplayWindow() const                        { return m_defaultDisplayWindow;                 }
  void              setDefaultDisplayWindow(Window& defaultDisplayWindow ) { m_defaultDisplayWindow = defaultDisplayWindow; }

  bool              getHrdParametersPresentFlag() const                    { return m_hrdParametersPresentFlag;             }
  void              setHrdParametersPresentFlag(bool i)                    { m_hrdParametersPresentFlag = i;                }

  bool              getBitstreamRestrictionFlag() const                    { return m_bitstreamRestrictionFlag;             }
  void              setBitstreamRestrictionFlag(bool i)                    { m_bitstreamRestrictionFlag = i;                }

#if HEVC_TILES_WPP
  bool              getTilesFixedStructureFlag() const                     { return m_tilesFixedStructureFlag;              }
  void              setTilesFixedStructureFlag(bool i)                     { m_tilesFixedStructureFlag = i;                 }
#endif

  bool              getMotionVectorsOverPicBoundariesFlag() const          { return m_motionVectorsOverPicBoundariesFlag;   }
  void              setMotionVectorsOverPicBoundariesFlag(bool i)          { m_motionVectorsOverPicBoundariesFlag = i;      }

  bool              getRestrictedRefPicListsFlag() const                   { return m_restrictedRefPicListsFlag;            }
  void              setRestrictedRefPicListsFlag(bool b)                   { m_restrictedRefPicListsFlag = b;               }

  Int               getMinSpatialSegmentationIdc() const                   { return m_minSpatialSegmentationIdc;            }
  void              setMinSpatialSegmentationIdc(Int i)                    { m_minSpatialSegmentationIdc = i;               }

  Int               getMaxBytesPerPicDenom() const                         { return m_maxBytesPerPicDenom;                  }
  void              setMaxBytesPerPicDenom(Int i)                          { m_maxBytesPerPicDenom = i;                     }

  Int               getMaxBitsPerMinCuDenom() const                        { return m_maxBitsPerMinCuDenom;                 }
  void              setMaxBitsPerMinCuDenom(Int i)                         { m_maxBitsPerMinCuDenom = i;                    }

  Int               getLog2MaxMvLengthHorizontal() const                   { return m_log2MaxMvLengthHorizontal;            }
  void              setLog2MaxMvLengthHorizontal(Int i)                    { m_log2MaxMvLengthHorizontal = i;               }

  Int               getLog2MaxMvLengthVertical() const                     { return m_log2MaxMvLengthVertical;              }
  void              setLog2MaxMvLengthVertical(Int i)                      { m_log2MaxMvLengthVertical = i;                 }

  HRD*              getHrdParameters()                                     { return &m_hrdParameters;                       }
  const HRD*        getHrdParameters()  const                              { return &m_hrdParameters;                       }

  TimingInfo*       getTimingInfo()                                        { return &m_timingInfo;                          }
  const TimingInfo* getTimingInfo() const                                  { return &m_timingInfo;                          }
};

/// SPS RExt class
class SPSRExt // Names aligned to text specification
{
private:
  bool             m_transformSkipRotationEnabledFlag;
  bool             m_transformSkipContextEnabledFlag;
  bool             m_rdpcmEnabledFlag[NUMBER_OF_RDPCM_SIGNALLING_MODES];
  bool             m_extendedPrecisionProcessingFlag;
  bool             m_intraSmoothingDisabledFlag;
  bool             m_highPrecisionOffsetsEnabledFlag;
  bool             m_persistentRiceAdaptationEnabledFlag;
  bool             m_cabacBypassAlignmentEnabledFlag;

public:
  SPSRExt();

  bool settingsDifferFromDefaults() const
  {
    return getTransformSkipRotationEnabledFlag()
        || getTransformSkipContextEnabledFlag()
        || getRdpcmEnabledFlag(RDPCM_SIGNAL_IMPLICIT)
        || getRdpcmEnabledFlag(RDPCM_SIGNAL_EXPLICIT)
        || getExtendedPrecisionProcessingFlag()
        || getIntraSmoothingDisabledFlag()
        || getHighPrecisionOffsetsEnabledFlag()
        || getPersistentRiceAdaptationEnabledFlag()
        || getCabacBypassAlignmentEnabledFlag();
  }


  bool getTransformSkipRotationEnabledFlag() const                                     { return m_transformSkipRotationEnabledFlag;     }
  void setTransformSkipRotationEnabledFlag(const bool value)                           { m_transformSkipRotationEnabledFlag = value;    }

  bool getTransformSkipContextEnabledFlag() const                                      { return m_transformSkipContextEnabledFlag;      }
  void setTransformSkipContextEnabledFlag(const bool value)                            { m_transformSkipContextEnabledFlag = value;     }

  bool getRdpcmEnabledFlag(const RDPCMSignallingMode signallingMode) const             { return m_rdpcmEnabledFlag[signallingMode];     }
  void setRdpcmEnabledFlag(const RDPCMSignallingMode signallingMode, const bool value) { m_rdpcmEnabledFlag[signallingMode] = value;    }

  bool getExtendedPrecisionProcessingFlag() const                                      { return m_extendedPrecisionProcessingFlag;      }
  void setExtendedPrecisionProcessingFlag(bool value)                                  { m_extendedPrecisionProcessingFlag = value;     }

  bool getIntraSmoothingDisabledFlag() const                                           { return m_intraSmoothingDisabledFlag;           }
  void setIntraSmoothingDisabledFlag(bool bValue)                                      { m_intraSmoothingDisabledFlag=bValue;           }

  bool getHighPrecisionOffsetsEnabledFlag() const                                      { return m_highPrecisionOffsetsEnabledFlag;      }
  void setHighPrecisionOffsetsEnabledFlag(bool value)                                  { m_highPrecisionOffsetsEnabledFlag = value;     }

  bool getPersistentRiceAdaptationEnabledFlag() const                                  { return m_persistentRiceAdaptationEnabledFlag;  }
  void setPersistentRiceAdaptationEnabledFlag(const bool value)                        { m_persistentRiceAdaptationEnabledFlag = value; }

  bool getCabacBypassAlignmentEnabledFlag() const                                      { return m_cabacBypassAlignmentEnabledFlag;      }
  void setCabacBypassAlignmentEnabledFlag(const bool value)                            { m_cabacBypassAlignmentEnabledFlag = value;     }
};


class SPS;
class SPSNext
{
private:
  SPS&  m_SPS;

  bool              m_NextEnabled;
#if JEM_TOOLS && !JVET_K0371_ALF
  bool              m_GALFEnabled;
#endif
  //=====  tool enabling flags (4 bytes - NOTE: last flag must be used for new extensions) =====
  bool              m_QTBT;                       // 1
#if JEM_TOOLS
  bool              m_NSST;                       // 2
  bool              m_Intra4Tap;                  // 3
#if !INTRA67_3MPM
  bool              m_Intra65Ang;                 // 4
#endif
#endif
  bool              m_LargeCTU;                   // 5
#if JEM_TOOLS
  bool              m_IntraBoundaryFilter;        // 6
  bool              m_SubPuMvp;                   // 7
#endif
#if !JEM_TOOLS && JVET_K0346
  bool              m_SubPuMvp;
#endif
#if JEM_TOOLS
  bool              m_ModifiedCABACEngine;        // 8
#endif
#if JVET_K0357_AMVR
  bool              m_IMV;                        // 9
#endif
#if JVET_K0072
#else
#if JEM_TOOLS
  bool              m_altResiComp;                // 10
#endif
#endif
#if JEM_TOOLS
  bool              m_highPrecMv;                 // 11
  bool              m_BIO;                        // 12
#endif
#if !JEM_TOOLS && (JVET_K0346 || JVET_K_AFFINE)
  bool              m_highPrecMv;
#endif
  bool              m_DisableMotionCompression;   // 13
#if JEM_TOOLS
  bool              m_LICEnabled;                 // 14
  bool              m_IntraPDPC;                  // 15
#endif
#if !JVET_K0371_ALF
  bool              m_ALFEnabled;                 // 16
#endif
#if JEM_TOOLS||JVET_K0190
  bool              m_LMChroma;                   // 17
#endif
#if JEM_TOOLS || JVET_K1000_SIMPLIFIED_EMT
  bool              m_IntraEMT;                   // 18
  bool              m_InterEMT;                   // 19
#endif
#if JEM_TOOLS
  bool              m_OBMC;                       // 20
  bool              m_FRUC;                       // 21
  bool              m_Affine;                     // 22
#if JVET_K0337_AFFINE_6PARA
  bool              m_AffineType;
#endif
  bool              m_AClip;                      // 23
#endif
#if !JEM_TOOLS && JVET_K_AFFINE
  bool              m_Affine;
#if JVET_K0337_AFFINE_6PARA
  bool              m_AffineType;
#endif
#endif
#if JEM_TOOLS
  bool              m_CIPFEnabled;                // 24
#endif
#if JEM_TOOLS
  bool              m_BIF;                        // 25
#endif
#if JEM_TOOLS
  bool              m_DMVR;                       // 26
  bool              m_MDMS;                       // 27
#endif
  bool              m_MTTEnabled;                 //
#if ENABLE_WPP_PARALLELISM
  bool              m_NextDQP;
#endif

public:
  const static int  NumReservedFlags = 32 - 27; /* current number of tool enabling flags */

private:
  //=====  additional parameters  =====
  // qtbt
  unsigned    m_CTUSize;
  unsigned    m_minQT[3];   // 0: I slice luma; 1: P/B slice; 2: I slice chroma
  unsigned    m_maxBTDepth[3];
  unsigned    m_maxBTSize[3];
  unsigned    m_dualITree;
#if JEM_TOOLS || JVET_K0346
  // sub-pu merging
  unsigned    m_subPuLog2Size;
  int         m_subPuMrgMode;
#endif
#if JEM_TOOLS
  // cabac engine
  unsigned    m_CABACEngineMode;
#endif
#if JVET_K0357_AMVR
  //imv
  ImvMode     m_ImvMode;
#endif
#if JVET_K0072
#else
#if JEM_TOOLS
  // ARC
  unsigned    m_altResiCompId;
#endif
#endif
#if JEM_TOOLS
  // local illumination compensation
  unsigned    m_LICMode;
#endif
  // multi type tree (QTBT + triple split)
  unsigned    m_MTTMode;
#if JEM_TOOLS
  int         m_OBMCBlkSize;

  unsigned    m_FRUCRefineFilter;
  unsigned    m_FRUCRefineRange;
  unsigned    m_FRUCSmallBlkRefineDepth;

  // adaptive clipping
  unsigned    m_AClipQuant;
#endif

#if JEM_TOOLS&&!JVET_K0190
  int         m_ELMMode;
#endif
#if JEM_TOOLS
  int         m_IntraPDPCMode;
#endif
#if JEM_TOOLS
  unsigned    m_CIPFMode;
#endif
  // ADD_NEW_TOOL : (sps extension) add tool enabling flags and associated parameters here

public:
  SPSNext( SPS& sps );

  const SPS&  getSPS              () const                                          { return m_SPS; }
  SPS&        getSPS              ()                                                { return m_SPS; }
  bool        nextToolsEnabled    () const                                          { return m_NextEnabled; }
  void        setNextToolsEnabled ( bool next )                                     { m_NextEnabled = next; }

  //=====  tool enabling flags and extension bit  =====
  void      setUseQTBT            ( bool QTBT )                                     { m_QTBT = QTBT; }
  bool      getUseQTBT            ()                                      const     { return m_QTBT; }
#if JEM_TOOLS
  void      setUseNSST            ( bool b )                                        { m_NSST = b; }
  bool      getUseNSST            ()                                      const     { return m_NSST; }
  void      setUseIntra4Tap       ( bool b )                                        { m_Intra4Tap = b; }
  bool      getUseIntra4Tap       ()                                      const     { return m_Intra4Tap; }
#if !INTRA67_3MPM
  void      setUseIntra65Ang      ( bool b )                                        { m_Intra65Ang = b; }
  bool      getUseIntra65Ang      ()                                      const     { return m_Intra65Ang; }
#endif
#endif
  void      setUseLargeCTU        ( bool b )                                        { m_LargeCTU = b; }
  bool      getUseLargeCTU        ()                                      const     { return m_LargeCTU; }
#if JEM_TOOLS
  void      setUseIntraBoundaryFilter( bool b )                                     { m_IntraBoundaryFilter = b; }
  bool      getUseIntraBoundaryFilter()                                   const     { return m_IntraBoundaryFilter; }

  bool      getUseSubPuMvp        ()                                      const     { return m_SubPuMvp; }
  void      setSubPuMvpMode       ( int n )                                         { m_subPuMrgMode = n; m_SubPuMvp = n != 0; }
  bool      getUseATMVP           ()                                      const     { return ( m_subPuMrgMode & 1 ) == 1; }
  bool      getUseSTMVP           ()                                      const     { return ( m_subPuMrgMode & 2 ) == 2; }
#endif
#if !JEM_TOOLS && JVET_K0346
  bool      getUseSubPuMvp()                                   const { return m_SubPuMvp; }
  void      setSubPuMvpMode(int n)                             { m_subPuMrgMode = n; m_SubPuMvp = n != 0; }
  bool      getUseATMVP()                                      const { return (m_subPuMrgMode & 1) == 1; }
#endif
#if JEM_TOOLS
  bool      getModifiedCABACEngine()                                      const     { return m_ModifiedCABACEngine; }
#endif
#if JVET_K0357_AMVR
  void      setUseIMV             ( bool b )                                        { m_IMV = b; }
  bool      getUseIMV             ()                                      const     { return m_IMV; }
#endif
#if JEM_TOOLS
  void      setUseAffine          ( bool b )                                        { m_Affine = b; }
  bool      getUseAffine          ()                                      const     { return m_Affine; }
#if JVET_K0337_AFFINE_6PARA
  void      setUseAffineType      ( bool b )                                        { m_AffineType = b; }
  bool      getUseAffineType      ()                                      const     { return m_AffineType; }
#endif
#endif
#if !JEM_TOOLS && JVET_K_AFFINE
  void      setUseAffine          ( bool b )                                        { m_Affine = b; }
  bool      getUseAffine          ()                                      const     { return m_Affine; }
#if JVET_K0337_AFFINE_6PARA
  void      setUseAffineType      ( bool b )                                        { m_AffineType = b; }
  bool      getUseAffineType      ()                                      const     { return m_AffineType; }
#endif
#endif
#if JVET_K0072
#else
#if JEM_TOOLS
  void      setUseAltResiComp     ( bool b )                                        { m_altResiComp = b; }
  bool      getUseAltResiComp     ()                                      const     { return m_altResiComp; }
#endif
#endif
#if JEM_TOOLS
  void      setUseHighPrecMv      ( bool b )                                        { m_highPrecMv = b; }
  bool      getUseHighPrecMv      ()                                      const     { return m_highPrecMv; }
  void      setUseBIO             ( bool b )                                        { m_BIO = b; }
  bool      getUseBIO             ()                                      const     { return m_BIO; }
#endif
#if !JEM_TOOLS && (JVET_K0346 || JVET_K_AFFINE)
  void      setUseHighPrecMv(bool b) { m_highPrecMv = b; }
  bool      getUseHighPrecMv()                                      const { return m_highPrecMv; }
#endif
  void      setDisableMotCompress ( bool b )                                        { m_DisableMotionCompression = b; }
  bool      getDisableMotCompress ()                                      const     { return m_DisableMotionCompression; }
#if JEM_TOOLS
  bool      getLICEnabled         ()                                      const     { return m_LICEnabled; }
#endif
  bool      getMTTEnabled         ()                                      const     { return m_MTTEnabled; }
#if ENABLE_WPP_PARALLELISM
  void      setUseNextDQP         ( bool b )                                        { m_NextDQP = b; }
  bool      getUseNextDQP         ()                                      const     { return m_NextDQP; }
#endif
#if JEM_TOOLS
  void      setUseIntraPDPC       ( bool b )                                        { m_IntraPDPC = b; }
  bool      getUseIntraPDPC       ()                                      const     { return m_IntraPDPC; }
#if !JVET_K0371_ALF
  void      setALFEnabled         ( bool b )                                        { m_ALFEnabled = b; }
  bool      getALFEnabled         ()                                      const     { return m_ALFEnabled; }
  void      setGALFEnabled        ( bool b )                                        { m_GALFEnabled = b; }
  bool      getGALFEnabled        ()                                      const     { return m_GALFEnabled; }
#endif
#endif
#if JEM_TOOLS||JVET_K0190
  void      setUseLMChroma        ( bool b )                                        { m_LMChroma = b; }
  bool      getUseLMChroma        ()                                      const     { return m_LMChroma; }
#endif
#if JEM_TOOLS || JVET_K1000_SIMPLIFIED_EMT
  void      setUseIntraEMT        ( bool b )                                        { m_IntraEMT = b; }
  bool      getUseIntraEMT        ()                                      const     { return m_IntraEMT; }
  void      setUseInterEMT        ( bool b )                                        { m_InterEMT = b; }
  bool      getUseInterEMT        ()                                      const     { return m_InterEMT; }
#endif
#if JEM_TOOLS
  void      setUseOBMC            ( bool b )                                        { m_OBMC = b; }
  bool      getUseOBMC            ()                                      const     { return m_OBMC; }
  void      setOBMCBlkSize        ( unsigned n )                                    { m_OBMCBlkSize = n; }
  int       getOBMCBlkSize        ()                                      const     { return m_OBMCBlkSize; }
#endif
#if JEM_TOOLS
  void      setCIPFMode           ( unsigned n )                                    { m_CIPFMode = n; m_CIPFEnabled = (m_CIPFMode>0); }
  unsigned  getCIPFMode           ()                                      const     { return m_CIPFMode; }
#endif
#if JEM_TOOLS
  void      setUseBIF             ( bool b )                                        { m_BIF = b; }
  bool      getUseBIF             ()                                      const     { return m_BIF; }
  void      setUseAClip           ( bool b )                                        { m_AClip = b; }
  bool      getUseAClip           ()                                      const     { return m_AClip; }
#endif
#if JEM_TOOLS
  void      setUseDMVR            ( bool b )                                        { m_DMVR = b; }
  bool      getUseDMVR            ()                                      const     { return m_DMVR; }
#endif
#if JEM_TOOLS
  void      setUseMDMS            ( bool b )                                        { m_MDMS = b; }
  bool      getUseMDMS            ()                                      const     { return m_MDMS; }
#endif

  //=====  additional parameters  =====
  // qtbt
  void      setCTUSize            ( unsigned    ctuSize )                           { m_CTUSize = ctuSize; }
  unsigned  getCTUSize            ()                                      const     { return  m_CTUSize;   }
  void      setMinQTSizes         ( unsigned*   minQT )                             { m_minQT[0] = minQT[0]; m_minQT[1] = minQT[1]; m_minQT[2] = minQT[2]; }
  unsigned  getMinQTSize          ( SliceType   slicetype,
                                    ChannelType chType = CHANNEL_TYPE_LUMA )
                                                                          const     { return slicetype == I_SLICE ? ( chType == CHANNEL_TYPE_LUMA ? m_minQT[0] : m_minQT[2] ) : m_minQT[1]; }
  void      setMaxBTDepth         ( unsigned    maxBTDepth,
                                    unsigned    maxBTDepthI,
                                    unsigned    maxBTDepthIChroma )                 { m_maxBTDepth[1] = maxBTDepth; m_maxBTDepth[0] = maxBTDepthI; m_maxBTDepth[2] = maxBTDepthIChroma; }
  unsigned  getMaxBTDepth         ()                                      const     { return m_maxBTDepth[1]; }
  unsigned  getMaxBTDepthI        ()                                      const     { return m_maxBTDepth[0]; }
  unsigned  getMaxBTDepthIChroma  ()                                      const     { return m_maxBTDepth[2]; }
  void      setMaxBTSize          ( unsigned    maxBTSize,
                                    unsigned    maxBTSizeI,
                                    unsigned    maxBTSizeC )                        { m_maxBTSize[1] = maxBTSize; m_maxBTSize[0] = maxBTSizeI; m_maxBTSize[2] = maxBTSizeC; }
  unsigned  getMaxBTSize          ()                                      const     { return m_maxBTSize[1]; }
  unsigned  getMaxBTSizeI         ()                                      const     { return m_maxBTSize[0]; }
  unsigned  getMaxBTSizeIChroma   ()                                      const     { return m_maxBTSize[2]; }

  void      setUseDualITree       ( bool b )                                        { m_dualITree = b; }
  bool      getUseDualITree       ()                                      const     { return m_dualITree; }

#if JEM_TOOLS || JVET_K0346
  // sub pu tmvp
  void      setSubPuMvpLog2Size   ( unsigned    log2Size )                          { m_subPuLog2Size = log2Size; }
  unsigned  getSubPuMvpLog2Size   ()                                      const     { return m_subPuLog2Size; }
#endif
#if JEM_TOOLS
  // cabac engine
  unsigned  getCABACEngineMode    ()                                      const     { return m_CABACEngineMode; }
  void      setCABACEngineMode    ( unsigned    mode )                              { m_CABACEngineMode = mode; m_ModifiedCABACEngine = ( m_CABACEngineMode != 0 ); }
#endif
#if JVET_K0357_AMVR
  void      setImvMode(ImvMode m) { m_ImvMode = m; m_IMV = m != 0;  }
  ImvMode   getImvMode            ()                                      const     { return m_ImvMode; }
#endif

#if JVET_K0072
#else
#if JEM_TOOLS
  // ARC
  void      setAltResiCompId      ( unsigned    mode )                              { m_altResiCompId = mode; }
  unsigned  getAltResiCompId      ()                                      const     { return m_altResiCompId; }
#endif
#endif

#if JEM_TOOLS
  // local illumination compensation
  unsigned  getLICMode            ()                                      const     { return m_LICMode; }
  void      setLICMode            ( unsigned    mode )                              { m_LICMode = mode; m_LICEnabled = ( m_LICMode != 0 ); }
#endif

  // multi type tree
  unsigned  getMTTMode            ()                                      const     { return m_MTTMode; }
  void      setMTTMode            ( unsigned    mode )                              { m_MTTMode = mode; m_MTTEnabled = ( m_MTTMode != 0 ); }

#if JEM_TOOLS
  void      setUseFRUCMrgMode     ( bool b )                                        { m_FRUC = b; }
  unsigned  getUseFRUCMrgMode     ()                                      const     { return m_FRUC; }
  void      setFRUCRefineFilter   ( unsigned n )                                    { m_FRUCRefineFilter = n; }
  unsigned  getFRUCRefineFilter   ()                                      const     { return m_FRUCRefineFilter; }
  void      setFRUCRefineRange    ( unsigned n )                                    { m_FRUCRefineRange = n; }
  unsigned  getFRUCRefineRange    ()                                      const     { return m_FRUCRefineRange; }
  void      setFRUCSmallBlkRefineDepth( unsigned n )                                { m_FRUCSmallBlkRefineDepth = n; }
  unsigned  getFRUCSmallBlkRefineDepth()                                  const     { return m_FRUCSmallBlkRefineDepth; }

  //adaptive clipping
  void      setAClipQuant         ( unsigned n )                                    { m_AClipQuant = n; }
  unsigned  getAClipQuant         ()                                      const     { return m_AClipQuant; }
#endif
#if JEM_TOOLS
#if !JVET_K0190
  void      setELMMode            ( int m )                                         { m_ELMMode = m; }
  int       getELMMode()                                                  const     { return m_ELMMode; }
  bool      isELMModeMMLM()                                               const     { return 0 != (m_ELMMode & 1); }
  bool      isELMModeMFLM()                                               const     { return 0 != (m_ELMMode & 2); }
  int       getRealNumIntraMode()                                         const     { return 68 + (m_ELMMode & 1) + ((m_ELMMode & 2) << 1); }
#endif
#endif
#if JEM_TOOLS
  void      setIntraPDPCMode      ( int n )                                         { m_IntraPDPCMode = n; }
  int       getIntraPDPCMode      ()                                      const     { return m_IntraPDPCMode; }
  bool      isIntraPDPC           ()                                      const     { return 0 != (m_IntraPDPCMode&1); }
  bool      isPlanarPDPC          ()                                      const     { return 0 != (m_IntraPDPCMode&2); }
#endif
  // ADD_NEW_TOOL : (sps extension) add access functions for tool enabling flags and associated parameters here

};


/// SPS class
class SPS
{
private:
  Int               m_SPSId;
#if HEVC_VPS
  Int               m_VPSId;
#endif
  ChromaFormat      m_chromaFormatIdc;

  UInt              m_uiMaxTLayers;           // maximum number of temporal layers

  // Structure
  UInt              m_picWidthInLumaSamples;
  UInt              m_picHeightInLumaSamples;

  Int               m_log2MinCodingBlockSize;
  Int               m_log2DiffMaxMinCodingBlockSize;
  UInt              m_uiMaxCUWidth;
  UInt              m_uiMaxCUHeight;
  UInt              m_uiMaxCodingDepth; ///< Total CU depth, relative to the smallest possible transform block size.

  Window            m_conformanceWindow;

  RPSList           m_RPSList;
  bool              m_bLongTermRefsPresent;
  bool              m_SPSTemporalMVPEnabledFlag;
  Int               m_numReorderPics[MAX_TLAYER];

  // Tool list
  UInt              m_uiQuadtreeTULog2MaxSize;
  UInt              m_uiQuadtreeTULog2MinSize;
  UInt              m_uiQuadtreeTUMaxDepthInter;
  UInt              m_uiQuadtreeTUMaxDepthIntra;
  bool              m_usePCM;
  UInt              m_pcmLog2MaxSize;
  UInt              m_uiPCMLog2MinSize;
  bool              m_useAMP;

  // Parameter
  BitDepths         m_bitDepths;
  Int               m_qpBDOffset[MAX_NUM_CHANNEL_TYPE];
  Int               m_pcmBitDepths[MAX_NUM_CHANNEL_TYPE];
  bool              m_bPCMFilterDisableFlag;

  UInt              m_uiBitsForPOC;
  UInt              m_numLongTermRefPicSPS;
  UInt              m_ltRefPicPocLsbSps[MAX_NUM_LONG_TERM_REF_PICS];
  bool              m_usedByCurrPicLtSPSFlag[MAX_NUM_LONG_TERM_REF_PICS];
  // Max physical transform size
  UInt              m_uiMaxTrSize;

  bool              m_bUseSAO;

  bool              m_bTemporalIdNestingFlag; // temporal_id_nesting_flag

#if HEVC_USE_SCALING_LISTS
  bool              m_scalingListEnabledFlag;
  bool              m_scalingListPresentFlag;
  ScalingList       m_scalingList;
#endif
  UInt              m_uiMaxDecPicBuffering[MAX_TLAYER];
  UInt              m_uiMaxLatencyIncreasePlus1[MAX_TLAYER];

#if HEVC_USE_INTRA_SMOOTHING_T32 || HEVC_USE_INTRA_SMOOTHING_T64
  bool              m_useStrongIntraSmoothing;
#endif

  bool              m_vuiParametersPresentFlag;
  VUI               m_vuiParameters;

  SPSRExt           m_spsRangeExtension;
  SPSNext           m_spsNextExtension;

  static const Int  m_winUnitX[NUM_CHROMA_FORMAT];
  static const Int  m_winUnitY[NUM_CHROMA_FORMAT];
  PTL               m_pcPTL;

#if JVET_K0371_ALF
  bool              m_useALF;
#endif

public:

  SPS();
  virtual                 ~SPS();

#if HEVC_VPS
  Int                     getVPSId() const                                                                { return m_VPSId;                                                      }
  void                    setVPSId(Int i)                                                                 { m_VPSId = i;                                                         }
#endif
  Int                     getSPSId() const                                                                { return m_SPSId;                                                      }
  void                    setSPSId(Int i)                                                                 { m_SPSId = i;                                                         }
  ChromaFormat            getChromaFormatIdc () const                                                     { return m_chromaFormatIdc;                                            }
  void                    setChromaFormatIdc (ChromaFormat i)                                             { m_chromaFormatIdc = i;                                               }

  static Int              getWinUnitX (Int chromaFormatIdc)                                               { CHECK(chromaFormatIdc < 0 || chromaFormatIdc >= NUM_CHROMA_FORMAT, "Invalid chroma format parameter"); return m_winUnitX[chromaFormatIdc]; }
  static Int              getWinUnitY (Int chromaFormatIdc)                                               { CHECK(chromaFormatIdc < 0 || chromaFormatIdc >= NUM_CHROMA_FORMAT, "Invalid chroma format parameter"); return m_winUnitY[chromaFormatIdc]; }

  // structure
  void                    setPicWidthInLumaSamples( UInt u )                                              { m_picWidthInLumaSamples = u;                                         }
  UInt                    getPicWidthInLumaSamples() const                                                { return  m_picWidthInLumaSamples;                                     }
  void                    setPicHeightInLumaSamples( UInt u )                                             { m_picHeightInLumaSamples = u;                                        }
  UInt                    getPicHeightInLumaSamples() const                                               { return  m_picHeightInLumaSamples;                                    }

  Window&                 getConformanceWindow()                                                          { return  m_conformanceWindow;                                         }
  const Window&           getConformanceWindow() const                                                    { return  m_conformanceWindow;                                         }
  void                    setConformanceWindow(Window& conformanceWindow )                                { m_conformanceWindow = conformanceWindow;                             }

  UInt                    getNumLongTermRefPicSPS() const                                                 { return m_numLongTermRefPicSPS;                                       }
  void                    setNumLongTermRefPicSPS(UInt val)                                               { m_numLongTermRefPicSPS = val;                                        }

  UInt                    getLtRefPicPocLsbSps(UInt index) const                                          { CHECK( index >= MAX_NUM_LONG_TERM_REF_PICS, "Index exceeds boundary" ); return m_ltRefPicPocLsbSps[index]; }
  void                    setLtRefPicPocLsbSps(UInt index, UInt val)                                      { CHECK( index >= MAX_NUM_LONG_TERM_REF_PICS, "Index exceeds boundary" ); m_ltRefPicPocLsbSps[index] = val;  }

  bool                    getUsedByCurrPicLtSPSFlag(Int i) const                                          { CHECK( i >= MAX_NUM_LONG_TERM_REF_PICS, "Index exceeds boundary" ); return m_usedByCurrPicLtSPSFlag[i];    }
  void                    setUsedByCurrPicLtSPSFlag(Int i, bool x)                                        { CHECK( i >= MAX_NUM_LONG_TERM_REF_PICS, "Index exceeds boundary" ); m_usedByCurrPicLtSPSFlag[i] = x;       }

  Int                     getLog2MinCodingBlockSize() const                                               { return m_log2MinCodingBlockSize;                                     }
  void                    setLog2MinCodingBlockSize(Int val)                                              { m_log2MinCodingBlockSize = val;                                      }
  Int                     getLog2DiffMaxMinCodingBlockSize() const                                        { return m_log2DiffMaxMinCodingBlockSize;                              }
  void                    setLog2DiffMaxMinCodingBlockSize(Int val)                                       { m_log2DiffMaxMinCodingBlockSize = val;                               }

  void                    setMaxCUWidth( UInt u )                                                         { m_uiMaxCUWidth = u;                                                  }
  UInt                    getMaxCUWidth() const                                                           { return  m_uiMaxCUWidth;                                              }
  void                    setMaxCUHeight( UInt u )                                                        { m_uiMaxCUHeight = u;                                                 }
  UInt                    getMaxCUHeight() const                                                          { return  m_uiMaxCUHeight;                                             }
  void                    setMaxCodingDepth( UInt u )                                                     { m_uiMaxCodingDepth = u;                                              }
  UInt                    getMaxCodingDepth() const                                                       { return  m_uiMaxCodingDepth;                                          }
  void                    setUsePCM( bool b )                                                             { m_usePCM = b;                                                        }
  bool                    getUsePCM() const                                                               { return m_usePCM;                                                     }
  void                    setPCMLog2MaxSize( UInt u )                                                     { m_pcmLog2MaxSize = u;                                                }
  UInt                    getPCMLog2MaxSize() const                                                       { return  m_pcmLog2MaxSize;                                            }
  void                    setPCMLog2MinSize( UInt u )                                                     { m_uiPCMLog2MinSize = u;                                              }
  UInt                    getPCMLog2MinSize() const                                                       { return  m_uiPCMLog2MinSize;                                          }
  void                    setBitsForPOC( UInt u )                                                         { m_uiBitsForPOC = u;                                                  }
  UInt                    getBitsForPOC() const                                                           { return m_uiBitsForPOC;                                               }
  bool                    getUseAMP() const                                                               { return m_useAMP;                                                     }
  void                    setUseAMP( bool b )                                                             { m_useAMP = b;                                                        }
  void                    setQuadtreeTULog2MaxSize( UInt u )                                              { m_uiQuadtreeTULog2MaxSize = u;                                       }
  UInt                    getQuadtreeTULog2MaxSize() const                                                { return m_uiQuadtreeTULog2MaxSize;                                    }
  void                    setQuadtreeTULog2MinSize( UInt u )                                              { m_uiQuadtreeTULog2MinSize = u;                                       }
  UInt                    getQuadtreeTULog2MinSize() const                                                { return m_uiQuadtreeTULog2MinSize;                                    }
  void                    setQuadtreeTUMaxDepthInter( UInt u )                                            { m_uiQuadtreeTUMaxDepthInter = u;                                     }
  void                    setQuadtreeTUMaxDepthIntra( UInt u )                                            { m_uiQuadtreeTUMaxDepthIntra = u;                                     }
  UInt                    getQuadtreeTUMaxDepthInter() const                                              { return m_uiQuadtreeTUMaxDepthInter;                                  }
  UInt                    getQuadtreeTUMaxDepthIntra() const                                              { return m_uiQuadtreeTUMaxDepthIntra;                                  }
  void                    setNumReorderPics(Int i, UInt tlayer)                                           { m_numReorderPics[tlayer] = i;                                        }
  Int                     getNumReorderPics(UInt tlayer) const                                            { return m_numReorderPics[tlayer];                                     }
  void                    createRPSList( Int numRPS );
  const RPSList*          getRPSList() const                                                              { return &m_RPSList;                                                   }
  RPSList*                getRPSList()                                                                    { return &m_RPSList;                                                   }
  bool                    getLongTermRefsPresent() const                                                  { return m_bLongTermRefsPresent;                                       }
  void                    setLongTermRefsPresent(bool b)                                                  { m_bLongTermRefsPresent=b;                                            }
  bool                    getSPSTemporalMVPEnabledFlag() const                                            { return m_SPSTemporalMVPEnabledFlag;                                  }
  void                    setSPSTemporalMVPEnabledFlag(bool b)                                            { m_SPSTemporalMVPEnabledFlag=b;                                       }
  // physical transform
  void                    setMaxTrSize( UInt u )                                                          { m_uiMaxTrSize = u;                                                   }
  UInt                    getMaxTrSize() const                                                            { return  m_uiMaxTrSize;                                               }

  // Bit-depth
  Int                     getBitDepth(ChannelType type) const                                             { return m_bitDepths.recon[type];                                      }
  void                    setBitDepth(ChannelType type, Int u )                                           { m_bitDepths.recon[type] = u;                                         }
  const BitDepths&        getBitDepths() const                                                            { return m_bitDepths;                                                  }
  Int                     getMaxLog2TrDynamicRange(ChannelType channelType) const                         { return getSpsRangeExtension().getExtendedPrecisionProcessingFlag() ? std::max<Int>(15, Int(m_bitDepths.recon[channelType] + 6)) : 15; }

  Int                     getDifferentialLumaChromaBitDepth() const                                       { return Int(m_bitDepths.recon[CHANNEL_TYPE_LUMA]) - Int(m_bitDepths.recon[CHANNEL_TYPE_CHROMA]); }
  Int                     getQpBDOffset(ChannelType type) const                                           { return m_qpBDOffset[type];                                           }
  void                    setQpBDOffset(ChannelType type, Int i)                                          { m_qpBDOffset[type] = i;                                              }

  void                    setUseSAO(bool bVal)                                                            { m_bUseSAO = bVal;                                                    }
  bool                    getUseSAO() const                                                               { return m_bUseSAO;                                                    }

  UInt                    getMaxTLayers() const                                                           { return m_uiMaxTLayers; }
  void                    setMaxTLayers( UInt uiMaxTLayers )                                              { CHECK( uiMaxTLayers > MAX_TLAYER, "Invalid number T-layers" ); m_uiMaxTLayers = uiMaxTLayers; }

  bool                    getTemporalIdNestingFlag() const                                                { return m_bTemporalIdNestingFlag;                                     }
  void                    setTemporalIdNestingFlag( bool bValue )                                         { m_bTemporalIdNestingFlag = bValue;                                   }
  UInt                    getPCMBitDepth(ChannelType type) const                                          { return m_pcmBitDepths[type];                                         }
  void                    setPCMBitDepth(ChannelType type, UInt u)                                        { m_pcmBitDepths[type] = u;                                            }
  void                    setPCMFilterDisableFlag( bool bValue )                                          { m_bPCMFilterDisableFlag = bValue;                                    }
  bool                    getPCMFilterDisableFlag() const                                                 { return m_bPCMFilterDisableFlag;                                      }

#if HEVC_USE_SCALING_LISTS
  bool                    getScalingListFlag() const                                                      { return m_scalingListEnabledFlag;                                     }
  void                    setScalingListFlag( bool b )                                                    { m_scalingListEnabledFlag  = b;                                       }
  bool                    getScalingListPresentFlag() const                                               { return m_scalingListPresentFlag;                                     }
  void                    setScalingListPresentFlag( bool b )                                             { m_scalingListPresentFlag  = b;                                       }
  ScalingList&            getScalingList()                                                                { return m_scalingList; }
  const ScalingList&      getScalingList() const                                                          { return m_scalingList; }
#endif
  UInt                    getMaxDecPicBuffering(UInt tlayer) const                                        { return m_uiMaxDecPicBuffering[tlayer];                               }
  void                    setMaxDecPicBuffering( UInt ui, UInt tlayer )                                   { CHECK(tlayer >= MAX_TLAYER, "Invalid T-layer"); m_uiMaxDecPicBuffering[tlayer] = ui;    }
  UInt                    getMaxLatencyIncreasePlus1(UInt tlayer) const                                   { return m_uiMaxLatencyIncreasePlus1[tlayer];                          }
  void                    setMaxLatencyIncreasePlus1( UInt ui , UInt tlayer)                              { m_uiMaxLatencyIncreasePlus1[tlayer] = ui;                            }

#if HEVC_USE_INTRA_SMOOTHING_T32 || HEVC_USE_INTRA_SMOOTHING_T64
  void                    setUseStrongIntraSmoothing(bool bVal)                                           { m_useStrongIntraSmoothing = bVal;                                    }
  bool                    getUseStrongIntraSmoothing() const                                              { return m_useStrongIntraSmoothing;                                    }

#endif
  bool                    getVuiParametersPresentFlag() const                                             { return m_vuiParametersPresentFlag;                                   }
  void                    setVuiParametersPresentFlag(bool b)                                             { m_vuiParametersPresentFlag = b;                                      }
  VUI*                    getVuiParameters()                                                              { return &m_vuiParameters;                                             }
  const VUI*              getVuiParameters() const                                                        { return &m_vuiParameters;                                             }
  const PTL*              getPTL() const                                                                  { return &m_pcPTL;                                                     }
  PTL*                    getPTL()                                                                        { return &m_pcPTL;                                                     }

  const SPSRExt&          getSpsRangeExtension() const                                                    { return m_spsRangeExtension;                                          }
  SPSRExt&                getSpsRangeExtension()                                                          { return m_spsRangeExtension;                                          }

  const SPSNext&          getSpsNext() const                                                              { return m_spsNextExtension;                                           }
  SPSNext&                getSpsNext()                                                                    { return m_spsNextExtension;                                           }

#if JVET_K0371_ALF
  bool                    getUseALF() const { return m_useALF; }
  void                    setUseALF( bool b ) { m_useALF = b; }
#endif
};


/// Reference Picture Lists class

class RefPicListModification
{
private:
  bool m_refPicListModificationFlagL0;
  bool m_refPicListModificationFlagL1;
  UInt m_RefPicSetIdxL0[REF_PIC_LIST_NUM_IDX];
  UInt m_RefPicSetIdxL1[REF_PIC_LIST_NUM_IDX];

public:
          RefPicListModification();
  virtual ~RefPicListModification();

  bool    getRefPicListModificationFlagL0() const        { return m_refPicListModificationFlagL0;                                  }
  void    setRefPicListModificationFlagL0(bool flag)     { m_refPicListModificationFlagL0 = flag;                                  }
  bool    getRefPicListModificationFlagL1() const        { return m_refPicListModificationFlagL1;                                  }
  void    setRefPicListModificationFlagL1(bool flag)     { m_refPicListModificationFlagL1 = flag;                                  }
  UInt    getRefPicSetIdxL0(UInt idx) const              { CHECK(idx>=REF_PIC_LIST_NUM_IDX, "Invalid ref-pic-list index"); return m_RefPicSetIdxL0[idx];         }
  void    setRefPicSetIdxL0(UInt idx, UInt refPicSetIdx) { CHECK(idx>=REF_PIC_LIST_NUM_IDX, "Invalid ref-pic-list index"); m_RefPicSetIdxL0[idx] = refPicSetIdx; }
  UInt    getRefPicSetIdxL1(UInt idx) const              { CHECK(idx>=REF_PIC_LIST_NUM_IDX, "Invalid ref-pic-list index"); return m_RefPicSetIdxL1[idx];         }
  void    setRefPicSetIdxL1(UInt idx, UInt refPicSetIdx) { CHECK(idx>=REF_PIC_LIST_NUM_IDX, "Invalid ref-pic-list index"); m_RefPicSetIdxL1[idx] = refPicSetIdx; }
};



/// PPS RExt class
class PPSRExt // Names aligned to text specification
{
private:
  Int              m_log2MaxTransformSkipBlockSize;
  bool             m_crossComponentPredictionEnabledFlag;

  // Chroma QP Adjustments
  Int              m_diffCuChromaQpOffsetDepth;
  Int              m_chromaQpOffsetListLen; // size (excludes the null entry used in the following array).
  ChromaQpAdj      m_ChromaQpAdjTableIncludingNullEntry[1+MAX_QP_OFFSET_LIST_SIZE]; //!< Array includes entry [0] for the null offset used when cu_chroma_qp_offset_flag=0, and entries [cu_chroma_qp_offset_idx+1...] otherwise

  UInt             m_log2SaoOffsetScale[MAX_NUM_CHANNEL_TYPE];

public:
  PPSRExt();

  bool settingsDifferFromDefaults(const bool bTransformSkipEnabledFlag) const
  {
    return (bTransformSkipEnabledFlag && (getLog2MaxTransformSkipBlockSize() !=2))
        || (getCrossComponentPredictionEnabledFlag() )
        || (getChromaQpOffsetListEnabledFlag() )
        || (getLog2SaoOffsetScale(CHANNEL_TYPE_LUMA) !=0 )
        || (getLog2SaoOffsetScale(CHANNEL_TYPE_CHROMA) !=0 );
  }

  UInt                   getLog2MaxTransformSkipBlockSize() const                         { return m_log2MaxTransformSkipBlockSize;         }
  void                   setLog2MaxTransformSkipBlockSize( UInt u )                       { m_log2MaxTransformSkipBlockSize  = u;           }

  bool                   getCrossComponentPredictionEnabledFlag() const                   { return m_crossComponentPredictionEnabledFlag;   }
  void                   setCrossComponentPredictionEnabledFlag(bool value)               { m_crossComponentPredictionEnabledFlag = value;  }

  void                   clearChromaQpOffsetList()                                        { m_chromaQpOffsetListLen = 0;                    }

  UInt                   getDiffCuChromaQpOffsetDepth () const                            { return m_diffCuChromaQpOffsetDepth;             }
  void                   setDiffCuChromaQpOffsetDepth ( UInt u )                          { m_diffCuChromaQpOffsetDepth = u;                }

  bool                   getChromaQpOffsetListEnabledFlag() const                         { return getChromaQpOffsetListLen()>0;            }
  Int                    getChromaQpOffsetListLen() const                                 { return m_chromaQpOffsetListLen;                 }

  const ChromaQpAdj&     getChromaQpOffsetListEntry( Int cuChromaQpOffsetIdxPlus1 ) const
  {
    CHECK(cuChromaQpOffsetIdxPlus1 >= m_chromaQpOffsetListLen+1, "Invalid chroma QP offset");
    return m_ChromaQpAdjTableIncludingNullEntry[cuChromaQpOffsetIdxPlus1]; // Array includes entry [0] for the null offset used when cu_chroma_qp_offset_flag=0, and entries [cu_chroma_qp_offset_idx+1...] otherwise
  }

  void                   setChromaQpOffsetListEntry( Int cuChromaQpOffsetIdxPlus1, Int cbOffset, Int crOffset )
  {
    CHECK(cuChromaQpOffsetIdxPlus1 == 0 || cuChromaQpOffsetIdxPlus1 > MAX_QP_OFFSET_LIST_SIZE, "Invalid chroma QP offset");
    m_ChromaQpAdjTableIncludingNullEntry[cuChromaQpOffsetIdxPlus1].u.comp.CbOffset = cbOffset; // Array includes entry [0] for the null offset used when cu_chroma_qp_offset_flag=0, and entries [cu_chroma_qp_offset_idx+1...] otherwise
    m_ChromaQpAdjTableIncludingNullEntry[cuChromaQpOffsetIdxPlus1].u.comp.CrOffset = crOffset;
    m_chromaQpOffsetListLen = std::max(m_chromaQpOffsetListLen, cuChromaQpOffsetIdxPlus1);
  }

  // Now: getPpsRangeExtension().getLog2SaoOffsetScale and getPpsRangeExtension().setLog2SaoOffsetScale
  UInt                   getLog2SaoOffsetScale(ChannelType type) const                    { return m_log2SaoOffsetScale[type];             }
  void                   setLog2SaoOffsetScale(ChannelType type, UInt uiBitShift)         { m_log2SaoOffsetScale[type] = uiBitShift;       }

};


/// PPS class
class PPS
{
private:
  Int              m_PPSId;                    // pic_parameter_set_id
  Int              m_SPSId;                    // seq_parameter_set_id
  Int              m_picInitQPMinus26;
  bool             m_useDQP;
  bool             m_bConstrainedIntraPred;    // constrained_intra_pred_flag
  bool             m_bSliceChromaQpFlag;       // slicelevel_chroma_qp_flag

  // access channel
  UInt             m_uiMaxCuDQPDepth;

  Int              m_chromaCbQpOffset;
  Int              m_chromaCrQpOffset;

  UInt             m_numRefIdxL0DefaultActive;
  UInt             m_numRefIdxL1DefaultActive;

  bool             m_bUseWeightPred;                    //!< Use of Weighting Prediction (P_SLICE)
  bool             m_useWeightedBiPred;                 //!< Use of Weighting Bi-Prediction (B_SLICE)
  bool             m_OutputFlagPresentFlag;             //!< Indicates the presence of output_flag in slice header
  bool             m_TransquantBypassEnabledFlag;       //!< Indicates presence of cu_transquant_bypass_flag in CUs.
  bool             m_useTransformSkip;
#if HEVC_DEPENDENT_SLICES
  bool             m_dependentSliceSegmentsEnabledFlag; //!< Indicates the presence of dependent slices
#endif
#if HEVC_TILES_WPP
  bool             m_tilesEnabledFlag;                  //!< Indicates the presence of tiles
  bool             m_entropyCodingSyncEnabledFlag;      //!< Indicates the presence of wavefronts

  bool             m_loopFilterAcrossTilesEnabledFlag;
  bool             m_uniformSpacingFlag;
  Int              m_numTileColumnsMinus1;
  Int              m_numTileRowsMinus1;
  std::vector<Int> m_tileColumnWidth;
  std::vector<Int> m_tileRowHeight;
#endif

#if JVET_K0072
#else
#if HEVC_USE_SIGN_HIDING
  bool             m_signDataHidingEnabledFlag;
#endif
#endif
  bool             m_cabacInitPresentFlag;

  bool             m_sliceHeaderExtensionPresentFlag;
  bool             m_loopFilterAcrossSlicesEnabledFlag;
  bool             m_deblockingFilterControlPresentFlag;
  bool             m_deblockingFilterOverrideEnabledFlag;
  bool             m_ppsDeblockingFilterDisabledFlag;
  Int              m_deblockingFilterBetaOffsetDiv2;    //< beta offset for deblocking filter
  Int              m_deblockingFilterTcOffsetDiv2;      //< tc offset for deblocking filter
#if HEVC_USE_SCALING_LISTS
  bool             m_scalingListPresentFlag;
  ScalingList      m_scalingList;                       //!< ScalingList class
#endif
  bool             m_listsModificationPresentFlag;
  UInt             m_log2ParallelMergeLevelMinus2;
  Int              m_numExtraSliceHeaderBits;

  PPSRExt          m_ppsRangeExtension;

public:
  PreCalcValues   *pcv;

public:
                         PPS();
  virtual                ~PPS();

  Int                    getPPSId() const                                                 { return m_PPSId;                               }
  void                   setPPSId(Int i)                                                  { m_PPSId = i;                                  }
  Int                    getSPSId() const                                                 { return m_SPSId;                               }
  void                   setSPSId(Int i)                                                  { m_SPSId = i;                                  }

  Int                    getPicInitQPMinus26() const                                      { return  m_picInitQPMinus26;                   }
  void                   setPicInitQPMinus26( Int i )                                     { m_picInitQPMinus26 = i;                       }
  bool                   getUseDQP() const                                                { return m_useDQP;                              }
  void                   setUseDQP( bool b )                                              { m_useDQP   = b;                               }
  bool                   getConstrainedIntraPred() const                                  { return  m_bConstrainedIntraPred;              }
  void                   setConstrainedIntraPred( bool b )                                { m_bConstrainedIntraPred = b;                  }
  bool                   getSliceChromaQpFlag() const                                     { return  m_bSliceChromaQpFlag;                 }
  void                   setSliceChromaQpFlag( bool b )                                   { m_bSliceChromaQpFlag = b;                     }

  void                   setMaxCuDQPDepth( UInt u )                                       { m_uiMaxCuDQPDepth = u;                        }
  UInt                   getMaxCuDQPDepth() const                                         { return m_uiMaxCuDQPDepth;                     }

  void                   setQpOffset(ComponentID compID, Int i )
  {
    if      (compID==COMPONENT_Cb)
    {
      m_chromaCbQpOffset = i;
    }
    else if (compID==COMPONENT_Cr)
    {
      m_chromaCrQpOffset = i;
    }
    else
    {
      THROW( "Invalid chroma QP offset" );
    }
  }
  Int                    getQpOffset(ComponentID compID) const
  {
    return (compID==COMPONENT_Y) ? 0 : (compID==COMPONENT_Cb ? m_chromaCbQpOffset : m_chromaCrQpOffset );
  }

  void                   setNumRefIdxL0DefaultActive(UInt ui)                             { m_numRefIdxL0DefaultActive=ui;                }
  UInt                   getNumRefIdxL0DefaultActive() const                              { return m_numRefIdxL0DefaultActive;            }
  void                   setNumRefIdxL1DefaultActive(UInt ui)                             { m_numRefIdxL1DefaultActive=ui;                }
  UInt                   getNumRefIdxL1DefaultActive() const                              { return m_numRefIdxL1DefaultActive;            }

  bool                   getUseWP() const                                                 { return m_bUseWeightPred;                      }
  bool                   getWPBiPred() const                                              { return m_useWeightedBiPred;                   }
  void                   setUseWP( bool b )                                               { m_bUseWeightPred = b;                         }
  void                   setWPBiPred( bool b )                                            { m_useWeightedBiPred = b;                      }

  void                   setOutputFlagPresentFlag( bool b )                               { m_OutputFlagPresentFlag = b;                  }
  bool                   getOutputFlagPresentFlag() const                                 { return m_OutputFlagPresentFlag;               }
  void                   setTransquantBypassEnabledFlag( bool b )                         { m_TransquantBypassEnabledFlag = b;            }
  bool                   getTransquantBypassEnabledFlag() const                           { return m_TransquantBypassEnabledFlag;         }

  bool                   getUseTransformSkip() const                                      { return m_useTransformSkip;                    }
  void                   setUseTransformSkip( bool b )                                    { m_useTransformSkip  = b;                      }

#if HEVC_TILES_WPP
  void                   setLoopFilterAcrossTilesEnabledFlag(bool b)                      { m_loopFilterAcrossTilesEnabledFlag = b;       }
  bool                   getLoopFilterAcrossTilesEnabledFlag() const                      { return m_loopFilterAcrossTilesEnabledFlag;    }
#endif
#if HEVC_DEPENDENT_SLICES
  bool                   getDependentSliceSegmentsEnabledFlag() const                     { return m_dependentSliceSegmentsEnabledFlag;   }
  void                   setDependentSliceSegmentsEnabledFlag(bool val)                   { m_dependentSliceSegmentsEnabledFlag = val;    }
#endif
#if HEVC_TILES_WPP
  bool                   getEntropyCodingSyncEnabledFlag() const                          { return m_entropyCodingSyncEnabledFlag;        }
  void                   setEntropyCodingSyncEnabledFlag(bool val)                        { m_entropyCodingSyncEnabledFlag = val;         }

  void                   setTilesEnabledFlag(bool val)                                    { m_tilesEnabledFlag = val;                     }
  bool                   getTilesEnabledFlag() const                                      { return m_tilesEnabledFlag;                    }
  void                   setTileUniformSpacingFlag(bool b)                                { m_uniformSpacingFlag = b;                     }
  bool                   getTileUniformSpacingFlag() const                                { return m_uniformSpacingFlag;                  }
  void                   setNumTileColumnsMinus1(Int i)                                   { m_numTileColumnsMinus1 = i;                   }
  Int                    getNumTileColumnsMinus1() const                                  { return m_numTileColumnsMinus1;                }
  void                   setTileColumnWidth(const std::vector<Int>& columnWidth )         { m_tileColumnWidth = columnWidth;              }
  UInt                   getTileColumnWidth(UInt columnIdx) const                         { return  m_tileColumnWidth[columnIdx];         }
  void                   setNumTileRowsMinus1(Int i)                                      { m_numTileRowsMinus1 = i;                      }
  Int                    getNumTileRowsMinus1() const                                     { return m_numTileRowsMinus1;                   }
  void                   setTileRowHeight(const std::vector<Int>& rowHeight)              { m_tileRowHeight = rowHeight;                  }
  UInt                   getTileRowHeight(UInt rowIdx) const                              { return m_tileRowHeight[rowIdx];               }
#endif

#if JVET_K0072
#else
#if HEVC_USE_SIGN_HIDING
  void                   setSignDataHidingEnabledFlag( bool b )                           { m_signDataHidingEnabledFlag = b;              }
  bool                   getSignDataHidingEnabledFlag() const                             { return m_signDataHidingEnabledFlag;           }
#endif
#endif
  void                   setCabacInitPresentFlag( bool flag )                             { m_cabacInitPresentFlag = flag;                }
  bool                   getCabacInitPresentFlag() const                                  { return m_cabacInitPresentFlag;                }
  void                   setDeblockingFilterControlPresentFlag( bool val )                { m_deblockingFilterControlPresentFlag = val;   }
  bool                   getDeblockingFilterControlPresentFlag() const                    { return m_deblockingFilterControlPresentFlag;  }
  void                   setDeblockingFilterOverrideEnabledFlag( bool val )               { m_deblockingFilterOverrideEnabledFlag = val;  }
  bool                   getDeblockingFilterOverrideEnabledFlag() const                   { return m_deblockingFilterOverrideEnabledFlag; }
  void                   setPPSDeblockingFilterDisabledFlag(bool val)                     { m_ppsDeblockingFilterDisabledFlag = val;      } //!< set offset for deblocking filter disabled
  bool                   getPPSDeblockingFilterDisabledFlag() const                       { return m_ppsDeblockingFilterDisabledFlag;     } //!< get offset for deblocking filter disabled
  void                   setDeblockingFilterBetaOffsetDiv2(Int val)                       { m_deblockingFilterBetaOffsetDiv2 = val;       } //!< set beta offset for deblocking filter
  Int                    getDeblockingFilterBetaOffsetDiv2() const                        { return m_deblockingFilterBetaOffsetDiv2;      } //!< get beta offset for deblocking filter
  void                   setDeblockingFilterTcOffsetDiv2(Int val)                         { m_deblockingFilterTcOffsetDiv2 = val;         } //!< set tc offset for deblocking filter
  Int                    getDeblockingFilterTcOffsetDiv2() const                          { return m_deblockingFilterTcOffsetDiv2;        } //!< get tc offset for deblocking filter
#if HEVC_USE_SCALING_LISTS
  bool                   getScalingListPresentFlag() const                                { return m_scalingListPresentFlag;              }
  void                   setScalingListPresentFlag( bool b )                              { m_scalingListPresentFlag  = b;                }
  ScalingList&           getScalingList()                                                 { return m_scalingList;                         }
  const ScalingList&     getScalingList() const                                           { return m_scalingList;                         }
#endif
  bool                   getListsModificationPresentFlag() const                          { return m_listsModificationPresentFlag;        }
  void                   setListsModificationPresentFlag( bool b )                        { m_listsModificationPresentFlag = b;           }
  UInt                   getLog2ParallelMergeLevelMinus2() const                          { return m_log2ParallelMergeLevelMinus2;        }
  void                   setLog2ParallelMergeLevelMinus2(UInt mrgLevel)                   { m_log2ParallelMergeLevelMinus2 = mrgLevel;    }
  Int                    getNumExtraSliceHeaderBits() const                               { return m_numExtraSliceHeaderBits;             }
  void                   setNumExtraSliceHeaderBits(Int i)                                { m_numExtraSliceHeaderBits = i;                }
  void                   setLoopFilterAcrossSlicesEnabledFlag( bool bValue )              { m_loopFilterAcrossSlicesEnabledFlag = bValue; }
  bool                   getLoopFilterAcrossSlicesEnabledFlag() const                     { return m_loopFilterAcrossSlicesEnabledFlag;   }
  bool                   getSliceHeaderExtensionPresentFlag() const                       { return m_sliceHeaderExtensionPresentFlag;     }
  void                   setSliceHeaderExtensionPresentFlag(bool val)                     { m_sliceHeaderExtensionPresentFlag = val;      }

  const PPSRExt&         getPpsRangeExtension() const                                     { return m_ppsRangeExtension;                   }
  PPSRExt&               getPpsRangeExtension()                                           { return m_ppsRangeExtension;                   }
};

struct WPScalingParam
{
  // Explicit weighted prediction parameters parsed in slice header,
  // or Implicit weighted prediction parameters (8 bits depth values).
  bool bPresentFlag;
  UInt uiLog2WeightDenom;
  Int  iWeight;
  Int  iOffset;

  // Weighted prediction scaling values built from above parameters (bitdepth scaled):
  Int  w;
  Int  o;
  Int  offset;
  Int  shift;
  Int  round;

};
struct WPACDCParam
{
  int64_t iAC;
  int64_t iDC;
};



/// slice header class
class Slice
{

private:
  //  Bitstream writing
  bool                       m_saoEnabledFlag[MAX_NUM_CHANNEL_TYPE];
  Int                        m_iPPSId;               ///< picture parameter set ID
  bool                       m_PicOutputFlag;        ///< pic_output_flag
  Int                        m_iPOC;
  Int                        m_iLastIDR;
  Int                        m_iAssociatedIRAP;
  NalUnitType                m_iAssociatedIRAPType;
  const ReferencePictureSet* m_pRPS;             //< pointer to RPS, either in the SPS or the local RPS in the same slice header
  ReferencePictureSet        m_localRPS;             //< RPS when present in slice header
  Int                        m_rpsIdx;               //< index of used RPS in the SPS or -1 for local RPS in the slice header
  RefPicListModification     m_RefPicListModification;
  NalUnitType                m_eNalUnitType;         ///< Nal unit type for the slice
  SliceType                  m_eSliceType;
  Int                        m_iSliceQp;
  Int                        m_iSliceQpBase;
#if HEVC_DEPENDENT_SLICES
  bool                       m_dependentSliceSegmentFlag;
#endif
  bool                       m_ChromaQpAdjEnabled;
  bool                       m_deblockingFilterDisable;
  bool                       m_deblockingFilterOverrideFlag;      //< offsets for deblocking filter inherit from PPS
  Int                        m_deblockingFilterBetaOffsetDiv2;    //< beta offset for deblocking filter
  Int                        m_deblockingFilterTcOffsetDiv2;      //< tc offset for deblocking filter
  Int                        m_list1IdxToList0Idx[MAX_NUM_REF];
  Int                        m_aiNumRefIdx   [NUM_REF_PIC_LIST_01];    //  for multiple reference of current slice
  bool                       m_pendingRasInit;

#if JEM_TOOLS
  bool                       m_bioLDBPossible;
  bool                       m_UseLIC;
#endif
#if JVET_K0072 
  bool                       m_depQuantEnabledFlag;
#if HEVC_USE_SIGN_HIDING
  bool                       m_signDataHidingEnabledFlag;
#endif
#endif
  bool                       m_bCheckLDC;

  //  Data
  Int                        m_iSliceQpDelta;
  Int                        m_iSliceChromaQpDelta[MAX_NUM_COMPONENT];
  Picture*                   m_apcRefPicList [NUM_REF_PIC_LIST_01][MAX_NUM_REF+1];
  Int                        m_aiRefPOCList  [NUM_REF_PIC_LIST_01][MAX_NUM_REF+1];
  bool                       m_bIsUsedAsLongTerm[NUM_REF_PIC_LIST_01][MAX_NUM_REF+1];
  Int                        m_iDepth;

#if JEM_TOOLS
  Int                        m_iFrucRefIdxPair[2][MAX_NUM_REF+1];
  bool                       m_bFrucRefIdxPairValid;
  Int                        m_iScaleFactor[256][256];
  bool                       m_bScaleFactorValid;
#endif

  // access channel
#if HEVC_VPS
  const VPS*                 m_pcVPS;
#endif
  const SPS*                 m_pcSPS;
  const PPS*                 m_pcPPS;
  Picture*                   m_pcPic;
  bool                       m_colFromL0Flag;  // collocated picture from List0 flag

  bool                       m_noOutputPriorPicsFlag;
  bool                       m_noRaslOutputFlag;
  bool                       m_handleCraAsBlaFlag;

  UInt                       m_colRefIdx;
  UInt                       m_maxNumMergeCand;

  double                     m_lambdas[MAX_NUM_COMPONENT];

  bool                       m_abEqualRef  [NUM_REF_PIC_LIST_01][MAX_NUM_REF][MAX_NUM_REF];
  UInt                       m_uiTLayer;
  bool                       m_bTLayerSwitchingFlag;

  SliceConstraint            m_sliceMode;
  UInt                       m_sliceArgument;
  UInt                       m_sliceCurStartCtuTsAddr;
  UInt                       m_sliceCurEndCtuTsAddr;
  UInt                       m_independentSliceIdx;
#if HEVC_DEPENDENT_SLICES
  UInt                       m_sliceSegmentIdx;
  SliceConstraint            m_sliceSegmentMode;
  UInt                       m_sliceSegmentArgument;
  UInt                       m_sliceSegmentCurStartCtuTsAddr;
  UInt                       m_sliceSegmentCurEndCtuTsAddr;
#endif
  bool                       m_nextSlice;
#if HEVC_DEPENDENT_SLICES
  bool                       m_nextSliceSegment;
#endif
  UInt                       m_sliceBits;
#if HEVC_DEPENDENT_SLICES
  UInt                       m_sliceSegmentBits;
#endif
  bool                       m_bFinalized;

  bool                       m_bTestWeightPred;
  bool                       m_bTestWeightBiPred;
  WPScalingParam             m_weightPredTable[NUM_REF_PIC_LIST_01][MAX_NUM_REF][MAX_NUM_COMPONENT]; // [REF_PIC_LIST_0 or REF_PIC_LIST_1][refIdx][0:Y, 1:U, 2:V]
  WPACDCParam                m_weightACDCParam[MAX_NUM_COMPONENT];
  ClpRngs                    m_clpRngs;
  std::vector<UInt>          m_substreamSizes;

  bool                       m_cabacInitFlag;
  int                        m_cabacWinUpdateMode;

  bool                       m_bLMvdL1Zero;
  bool                       m_temporalLayerNonReferenceFlag;
  bool                       m_LFCrossSliceBoundaryFlag;

  bool                       m_enableTMVPFlag;

#if JVET_K0346
  bool                       m_subPuMvpSubBlkSizeSliceEnable;
  int                        m_subPuMvpSubBlkLog2Size;
#endif

  SliceType                  m_encCABACTableIdx;           // Used to transmit table selection across slices.

  clock_t                    m_iProcessingStartTime;
  double                     m_dProcessingTime;
  UInt                       m_uiMaxBTSize;

#if JVET_K0371_ALF
  AlfSliceParam              m_alfSliceParam;
#endif

public:
                              Slice();
  virtual                     ~Slice();
  void                        initSlice();
  Int                         getRefIdx4MVPair( RefPicList eCurRefPicList, Int nCurRefIdx );
#if JEM_TOOLS
  inline Int                  getScaleFactor( Int iTDB, Int iTDD )                   { return Slice::m_iScaleFactor[128+iTDB][128+iTDD];                    }
#endif
#if HEVC_VPS
  void                        setVPS( VPS* pcVPS )                                   { m_pcVPS = pcVPS;                                              }
  const VPS*                  getVPS() const                                         { return m_pcVPS;                                               }
#endif
  void                        setSPS( const SPS* pcSPS )                             { m_pcSPS = pcSPS;                                              }
  const SPS*                  getSPS() const                                         { return m_pcSPS;                                               }

  void                        setPPS( const PPS* pcPPS )                             { m_pcPPS = pcPPS; m_iPPSId = (pcPPS) ? pcPPS->getPPSId() : -1; }
  const PPS*                  getPPS() const                                         { return m_pcPPS;                                               }

  void                        setPPSId( Int PPSId )                                  { m_iPPSId = PPSId;                                             }
  Int                         getPPSId() const                                       { return m_iPPSId;                                              }
  void                        setPicOutputFlag( bool b   )                           { m_PicOutputFlag = b;                                          }
  bool                        getPicOutputFlag() const                               { return m_PicOutputFlag;                                       }
  void                        setSaoEnabledFlag(ChannelType chType, bool s)          {m_saoEnabledFlag[chType] =s;                                   }
  bool                        getSaoEnabledFlag(ChannelType chType) const            { return m_saoEnabledFlag[chType];                              }
#if JEM_TOOLS
  void                        setBioLDBPossible( bool b )                            { m_bioLDBPossible = b;                                         }
  bool                        getBioLDBPossible() const                              { return m_bioLDBPossible;                                      }
#endif
  void                        setRPS( const ReferencePictureSet *pcRPS )             { m_pRPS = pcRPS;                                               }
  const ReferencePictureSet*  getRPS()                                               { return m_pRPS;                                                }
  ReferencePictureSet*        getLocalRPS()                                          { return &m_localRPS;                                           }

  void                        setRPSidx( Int rpsIdx )                                { m_rpsIdx = rpsIdx;                                            }
  Int                         getRPSidx() const                                      { return m_rpsIdx;                                              }
  RefPicListModification*     getRefPicListModification()                            { return &m_RefPicListModification;                             }
  void                        setLastIDR(Int iIDRPOC)                                { m_iLastIDR = iIDRPOC;                                         }
  Int                         getLastIDR() const                                     { return m_iLastIDR;                                            }
  void                        setAssociatedIRAPPOC(Int iAssociatedIRAPPOC)           { m_iAssociatedIRAP = iAssociatedIRAPPOC;                       }
  Int                         getAssociatedIRAPPOC() const                           { return m_iAssociatedIRAP;                                     }
  void                        setAssociatedIRAPType(NalUnitType associatedIRAPType)  { m_iAssociatedIRAPType = associatedIRAPType;                   }
  NalUnitType                 getAssociatedIRAPType() const                          { return m_iAssociatedIRAPType;                                 }
  SliceType                   getSliceType() const                                   { return m_eSliceType;                                          }
  Int                         getPOC() const                                         { return m_iPOC;                                                }
  Int                         getSliceQp() const                                     { return m_iSliceQp;                                            }
  bool                        getUseWeightedPrediction() const                       { return( (m_eSliceType==P_SLICE && testWeightPred()) || (m_eSliceType==B_SLICE && testWeightBiPred()) ); }
#if HEVC_DEPENDENT_SLICES
  bool                        getDependentSliceSegmentFlag() const                   { return m_dependentSliceSegmentFlag;                           }
  void                        setDependentSliceSegmentFlag(bool val)                 { m_dependentSliceSegmentFlag = val;                            }
#endif
  Int                         getSliceQpDelta() const                                { return m_iSliceQpDelta;                                       }
  Int                         getSliceChromaQpDelta(ComponentID compID) const        { return isLuma(compID) ? 0 : m_iSliceChromaQpDelta[compID];    }
  bool                        getUseChromaQpAdj() const                              { return m_ChromaQpAdjEnabled;                                  }
  bool                        getDeblockingFilterDisable() const                     { return m_deblockingFilterDisable;                             }
  bool                        getDeblockingFilterOverrideFlag() const                { return m_deblockingFilterOverrideFlag;                        }
  Int                         getDeblockingFilterBetaOffsetDiv2()const               { return m_deblockingFilterBetaOffsetDiv2;                      }
  Int                         getDeblockingFilterTcOffsetDiv2() const                { return m_deblockingFilterTcOffsetDiv2;                        }
  bool                        getPendingRasInit() const                              { return m_pendingRasInit;                                      }
  void                        setPendingRasInit( bool val )                          { m_pendingRasInit = val;                                       }

  Int                         getNumRefIdx( RefPicList e ) const                     { return m_aiNumRefIdx[e];                                      }
  Picture*                    getPic()                                               { return m_pcPic;                                               }
  const Picture*              getPic() const                                         { return m_pcPic;                                               }
  const Picture*              getRefPic( RefPicList e, Int iRefIdx) const            { return m_apcRefPicList[e][iRefIdx];                           }
  Int                         getRefPOC( RefPicList e, Int iRefIdx) const            { return m_aiRefPOCList[e][iRefIdx];                            }
  Int                         getDepth() const                                       { return m_iDepth;                                              }
  bool                        getColFromL0Flag() const                               { return m_colFromL0Flag;                                       }
  UInt                        getColRefIdx() const                                   { return m_colRefIdx;                                           }
  void                        checkColRefIdx(UInt curSliceSegmentIdx, const Picture* pic);
  bool                        getIsUsedAsLongTerm(Int i, Int j) const                { return m_bIsUsedAsLongTerm[i][j];                             }
  void                        setIsUsedAsLongTerm(Int i, Int j, bool value)          { m_bIsUsedAsLongTerm[i][j] = value;                            }
  bool                        getCheckLDC() const                                    { return m_bCheckLDC;                                           }
  bool                        getMvdL1ZeroFlag() const                               { return m_bLMvdL1Zero;                                         }
  Int                         getNumRpsCurrTempList() const;
  Int                         getList1IdxToList0Idx( Int list1Idx ) const            { return m_list1IdxToList0Idx[list1Idx];                        }
  bool                        isReferenceNalu() const                                { return ((getNalUnitType() <= NAL_UNIT_RESERVED_VCL_R15) && (getNalUnitType()%2 != 0)) || ((getNalUnitType() >= NAL_UNIT_CODED_SLICE_BLA_W_LP) && (getNalUnitType() <= NAL_UNIT_RESERVED_IRAP_VCL23) ); }
  void                        setPOC( Int i )                                        { m_iPOC              = i;                                      }
  void                        setNalUnitType( NalUnitType e )                        { m_eNalUnitType      = e;                                      }
  NalUnitType                 getNalUnitType() const                                 { return m_eNalUnitType;                                        }
  bool                        getRapPicFlag() const;
  bool                        getIdrPicFlag() const                                  { return getNalUnitType() == NAL_UNIT_CODED_SLICE_IDR_W_RADL || getNalUnitType() == NAL_UNIT_CODED_SLICE_IDR_N_LP; }
  bool                        isIRAP() const                                         { return (getNalUnitType() >= 16) && (getNalUnitType() <= 23);  }
  bool                        isIDRorBLA() const                                      { return (getNalUnitType() >= 16) && (getNalUnitType() <= 20);  }
  void                        checkCRA(const ReferencePictureSet *pReferencePictureSet, Int& pocCRA, NalUnitType& associatedIRAPType, PicList& rcListPic);
  void                        decodingRefreshMarking(Int& pocCRA, bool& bRefreshPending, PicList& rcListPic, const bool bEfficientFieldIRAPEnabled);
  void                        setSliceType( SliceType e )                            { m_eSliceType        = e;                                      }
  void                        setSliceQp( Int i )                                    { m_iSliceQp          = i;                                      }
  void                        setSliceQpDelta( Int i )                               { m_iSliceQpDelta     = i;                                      }
  void                        setSliceChromaQpDelta( ComponentID compID, Int i )     { m_iSliceChromaQpDelta[compID] = isLuma(compID) ? 0 : i;       }
  void                        setUseChromaQpAdj( bool b )                            { m_ChromaQpAdjEnabled = b;                                     }
  void                        setDeblockingFilterDisable( bool b )                   { m_deblockingFilterDisable= b;                                 }
  void                        setDeblockingFilterOverrideFlag( bool b )              { m_deblockingFilterOverrideFlag = b;                           }
  void                        setDeblockingFilterBetaOffsetDiv2( Int i )             { m_deblockingFilterBetaOffsetDiv2 = i;                         }
  void                        setDeblockingFilterTcOffsetDiv2( Int i )               { m_deblockingFilterTcOffsetDiv2 = i;                           }

  void                        setNumRefIdx( RefPicList e, Int i )                    { m_aiNumRefIdx[e]    = i;                                      }
  void                        setPic( Picture* p )                                   { m_pcPic             = p;                                      }
  void                        setDepth( Int iDepth )                                 { m_iDepth            = iDepth;                                 }

  void                        setRefPicList( PicList& rcListPic, bool checkNumPocTotalCurr = false, bool bCopyL0toL1ErrorCase = false );
  void                        setRefPOCList();

  void                        setColFromL0Flag( bool colFromL0 )                     { m_colFromL0Flag = colFromL0;                                  }
  void                        setColRefIdx( UInt refIdx)                             { m_colRefIdx = refIdx;                                         }
  void                        setCheckLDC( bool b )                                  { m_bCheckLDC = b;                                              }
  void                        setMvdL1ZeroFlag( bool b)                              { m_bLMvdL1Zero = b;                                            }

  bool                        isIntra() const                                        { return m_eSliceType == I_SLICE;                               }
  bool                        isInterB() const                                       { return m_eSliceType == B_SLICE;                               }
  bool                        isInterP() const                                       { return m_eSliceType == P_SLICE;                               }

  void                        setLambdas( const double lambdas[MAX_NUM_COMPONENT] )  { for (Int component = 0; component < MAX_NUM_COMPONENT; component++) m_lambdas[component] = lambdas[component]; }
  const double*               getLambdas() const                                     { return m_lambdas;                                             }

  void                        setMaxBTSize(Int i)                                    { m_uiMaxBTSize = i; }
  UInt                        getMaxBTSize() const                                   { return m_uiMaxBTSize; }

#if JEM_TOOLS
  bool                        getUseLIC()                                   const    { return m_UseLIC; }
  void                        setUseLIC( bool b )                                    { m_UseLIC = b; }
  void                        setUseLICOnPicLevel( bool fastPicDecision );
#endif
#if JVET_K0072
  void                        setDepQuantEnabledFlag( bool b )                       { m_depQuantEnabledFlag = b; }
  bool                        getDepQuantEnabledFlag() const                         { return m_depQuantEnabledFlag; }
#if HEVC_USE_SIGN_HIDING
  void                        setSignDataHidingEnabledFlag( bool b )                 { m_signDataHidingEnabledFlag = b;              }
  bool                        getSignDataHidingEnabledFlag() const                   { return m_signDataHidingEnabledFlag;           }
#endif
#endif

  void                        initEqualRef();
  bool                        isEqualRef( RefPicList e, Int iRefIdx1, Int iRefIdx2 )
  {
    CHECK(e>=NUM_REF_PIC_LIST_01, "Invalid reference picture list");
    if (iRefIdx1 < 0 || iRefIdx2 < 0)
    {
      return false;
    }
    else
    {
      return m_abEqualRef[e][iRefIdx1][iRefIdx2];
    }
  }

  void                        setEqualRef( RefPicList e, Int iRefIdx1, Int iRefIdx2, bool b)
  {
    CHECK( e >= NUM_REF_PIC_LIST_01, "Invalid reference picture list" );
    m_abEqualRef[e][iRefIdx1][iRefIdx2] = m_abEqualRef[e][iRefIdx2][iRefIdx1] = b;
  }

  static void                 sortPicList( PicList& rcListPic );
  void                        setList1IdxToList0Idx();

  UInt                        getTLayer() const                                      { return m_uiTLayer;                                            }
  void                        setTLayer( UInt uiTLayer )                             { m_uiTLayer = uiTLayer;                                        }

  void                        checkLeadingPictureRestrictions( PicList& rcListPic )                                         const;
  void                        applyReferencePictureSet( PicList& rcListPic, const ReferencePictureSet *RPSList)             const;
  bool                        isTemporalLayerSwitchingPoint( PicList& rcListPic )                                           const;
  bool                        isStepwiseTemporalLayerSwitchingPointCandidate( PicList& rcListPic )                          const;
  Int                         checkThatAllRefPicsAreAvailable( PicList& rcListPic, const ReferencePictureSet *pReferencePictureSet, bool printErrors, Int pocRandomAccess = 0, bool bUseRecoveryPoint = false) const;
  void                        createExplicitReferencePictureSetFromReference( PicList& rcListPic, const ReferencePictureSet *pReferencePictureSet, bool isRAP, Int pocRandomAccess, bool bUseRecoveryPoint, const bool bEfficientFieldIRAPEnabled);
  void                        setMaxNumMergeCand(UInt val )                          { m_maxNumMergeCand = val;                                      }
  UInt                        getMaxNumMergeCand() const                             { return m_maxNumMergeCand;                                     }

  void                        setNoOutputPriorPicsFlag( bool val )                   { m_noOutputPriorPicsFlag = val;                                }
  bool                        getNoOutputPriorPicsFlag() const                       { return m_noOutputPriorPicsFlag;                               }

  void                        setNoRaslOutputFlag( bool val )                        { m_noRaslOutputFlag = val;                                     }
  bool                        getNoRaslOutputFlag() const                            { return m_noRaslOutputFlag;                                    }

  void                        setHandleCraAsBlaFlag( bool val )                      { m_handleCraAsBlaFlag = val;                                   }
  bool                        getHandleCraAsBlaFlag() const                          { return m_handleCraAsBlaFlag;                                  }

  void                        setSliceMode( SliceConstraint mode )                   { m_sliceMode = mode;                                           }
  SliceConstraint             getSliceMode() const                                   { return m_sliceMode;                                           }
  void                        setSliceArgument( UInt uiArgument )                    { m_sliceArgument = uiArgument;                                 }
  UInt                        getSliceArgument() const                               { return m_sliceArgument;                                       }
  void                        setSliceCurStartCtuTsAddr( UInt ctuTsAddr )            { m_sliceCurStartCtuTsAddr = ctuTsAddr;                         } // CTU Tile-scan address (as opposed to raster-scan)
  UInt                        getSliceCurStartCtuTsAddr() const                      { return m_sliceCurStartCtuTsAddr;                              } // CTU Tile-scan address (as opposed to raster-scan)
  void                        setSliceCurEndCtuTsAddr( UInt ctuTsAddr )              { m_sliceCurEndCtuTsAddr = ctuTsAddr;                           } // CTU Tile-scan address (as opposed to raster-scan)
  UInt                        getSliceCurEndCtuTsAddr() const                        { return m_sliceCurEndCtuTsAddr;                                } // CTU Tile-scan address (as opposed to raster-scan)
  void                        setIndependentSliceIdx( UInt i)                        { m_independentSliceIdx = i;                                    }
  UInt                        getIndependentSliceIdx() const                         { return  m_independentSliceIdx;                                }
#if HEVC_DEPENDENT_SLICES
  void                        setSliceSegmentIdx( UInt i)                            { m_sliceSegmentIdx = i;                                        }
  UInt                        getSliceSegmentIdx() const                             { return  m_sliceSegmentIdx;                                    }
#endif
  void                        copySliceInfo(Slice *pcSliceSrc, bool cpyAlmostAll = true);
#if HEVC_DEPENDENT_SLICES
  void                        setSliceSegmentMode( SliceConstraint mode )            { m_sliceSegmentMode = mode;                                    }
  SliceConstraint             getSliceSegmentMode() const                            { return m_sliceSegmentMode;                                    }
  void                        setSliceSegmentArgument( UInt uiArgument )             { m_sliceSegmentArgument = uiArgument;                          }
  UInt                        getSliceSegmentArgument() const                        { return m_sliceSegmentArgument;                                }
#if HEVC_TILES_WPP
  void                        setSliceSegmentCurStartCtuTsAddr( UInt ctuTsAddr )     { m_sliceSegmentCurStartCtuTsAddr = ctuTsAddr;                  } // CTU Tile-scan address (as opposed to raster-scan)
  UInt                        getSliceSegmentCurStartCtuTsAddr() const               { return m_sliceSegmentCurStartCtuTsAddr;                       } // CTU Tile-scan address (as opposed to raster-scan)
  void                        setSliceSegmentCurEndCtuTsAddr( UInt ctuTsAddr )       { m_sliceSegmentCurEndCtuTsAddr = ctuTsAddr;                    } // CTU Tile-scan address (as opposed to raster-scan)
  UInt                        getSliceSegmentCurEndCtuTsAddr() const                 { return m_sliceSegmentCurEndCtuTsAddr;                         } // CTU Tile-scan address (as opposed to raster-scan)
#endif
#endif
  void                        setSliceBits( UInt uiVal )                             { m_sliceBits = uiVal;                                          }
  UInt                        getSliceBits() const                                   { return m_sliceBits;                                           }
#if HEVC_DEPENDENT_SLICES
  void                        setSliceSegmentBits( UInt uiVal )                      { m_sliceSegmentBits = uiVal;                                   }
  UInt                        getSliceSegmentBits() const                            { return m_sliceSegmentBits;                                    }
#endif
  void                        setFinalized( bool uiVal )                             { m_bFinalized = uiVal;                                         }
  bool                        getFinalized() const                                   { return m_bFinalized;                                          }
  bool                        testWeightPred( ) const                                { return m_bTestWeightPred;                                     }
  void                        setTestWeightPred( bool bValue )                       { m_bTestWeightPred = bValue;                                   }
  bool                        testWeightBiPred( ) const                              { return m_bTestWeightBiPred;                                   }
  void                        setTestWeightBiPred( bool bValue )                     { m_bTestWeightBiPred = bValue;                                 }
  void                        setWpScaling( WPScalingParam  wp[NUM_REF_PIC_LIST_01][MAX_NUM_REF][MAX_NUM_COMPONENT] )
  {
    memcpy(m_weightPredTable, wp, sizeof(WPScalingParam)*NUM_REF_PIC_LIST_01*MAX_NUM_REF*MAX_NUM_COMPONENT);
  }

  void                        getWpScaling( RefPicList e, Int iRefIdx, WPScalingParam *&wp) const;

  void                        resetWpScaling();
  void                        initWpScaling(const SPS *sps);

  void                        setWpAcDcParam( WPACDCParam wp[MAX_NUM_COMPONENT] )    { memcpy(m_weightACDCParam, wp, sizeof(WPACDCParam)*MAX_NUM_COMPONENT); }

  void                        getWpAcDcParam( const WPACDCParam *&wp ) const;
  void                        initWpAcDcParam();

  void                        clearSubstreamSizes( )                                 { return m_substreamSizes.clear();                              }
  UInt                        getNumberOfSubstreamSizes( )                           { return (UInt) m_substreamSizes.size();                        }
  void                        addSubstreamSize( UInt size )                          { m_substreamSizes.push_back(size);                             }
  UInt                        getSubstreamSize( UInt idx )                           { CHECK(idx>=getNumberOfSubstreamSizes(),"Invalid index"); return m_substreamSizes[idx]; }

  void                        setCabacInitFlag( bool val )                           { m_cabacInitFlag = val;                                        } //!< set CABAC initial flag
  bool                        getCabacInitFlag()                               const { return m_cabacInitFlag;                                       } //!< get CABAC initial flag
  void                        setCabacWinUpdateMode( int mode )                      { m_cabacWinUpdateMode = mode;                                  }
  int                         getCabacWinUpdateMode()                          const { return m_cabacWinUpdateMode;                                  }
  bool                        getTemporalLayerNonReferenceFlag()               const { return m_temporalLayerNonReferenceFlag;                       }
  void                        setTemporalLayerNonReferenceFlag(bool x)               { m_temporalLayerNonReferenceFlag = x;                          }
  void                        setLFCrossSliceBoundaryFlag( bool   val )              { m_LFCrossSliceBoundaryFlag = val;                             }
  bool                        getLFCrossSliceBoundaryFlag()                    const { return m_LFCrossSliceBoundaryFlag;                            }

  void                        setEnableTMVPFlag( bool   b )                          { m_enableTMVPFlag = b;                                         }
  bool                        getEnableTMVPFlag() const                              { return m_enableTMVPFlag;                                      }

  void                        setEncCABACTableIdx( SliceType idx )                   { m_encCABACTableIdx = idx;                                     }
  SliceType                   getEncCABACTableIdx() const                            { return m_encCABACTableIdx;                                    }

#if JVET_K0346
  void                        setSubPuMvpSliceSubblkSizeEnable(bool b) { m_subPuMvpSubBlkSizeSliceEnable = b; }
  bool                        getSubPuMvpSliceSubblkSizeEnable()                  const { return m_subPuMvpSubBlkSizeSliceEnable; }
  void                        setSubPuMvpSubblkLog2Size(int n) { m_subPuMvpSubBlkLog2Size = n; }
  int                         getSubPuMvpSubblkLog2Size()                         const { return m_subPuMvpSubBlkLog2Size; }
#endif

  void                        setSliceQpBase( Int i )                                { m_iSliceQpBase = i;                                           }
  Int                         getSliceQpBase()                                 const { return m_iSliceQpBase;                                        }

  void                        setDefaultClpRng( const SPS& sps );
  const ClpRngs&              clpRngs()                                         const { return m_clpRngs;}
  const ClpRng&               clpRng( ComponentID id)                           const { return m_clpRngs.comp[id];}
  ClpRngs&                    getClpRngs()                                            { return m_clpRngs;}
  unsigned                    getMinPictureDistance()                           const ;
  void startProcessingTimer();
  void stopProcessingTimer();
  void resetProcessingTime()       { m_dProcessingTime = m_iProcessingStartTime = 0; }
  double getProcessingTime() const { return m_dProcessingTime; }

#if JVET_K0371_ALF
  void                        setAlfSliceParam( AlfSliceParam& alfSliceParam ) { m_alfSliceParam = alfSliceParam; }
  AlfSliceParam&              getAlfSliceParam() { return m_alfSliceParam; }
#endif

protected:
  Picture*              xGetRefPic        (PicList& rcListPic, Int poc);
  Picture*              xGetLongTermRefPic(PicList& rcListPic, Int poc, bool pocHasMsb);
};// END CLASS DEFINITION Slice





void calculateParameterSetChangedFlag(bool &bChanged, const std::vector<uint8_t> *pOldData, const std::vector<uint8_t> *pNewData);

template <class T> class ParameterSetMap
{
public:
  template <class Tm>
  struct MapData
  {
    bool                  bChanged;
    std::vector<uint8_t>   *pNaluData; // Can be null
    Tm*                   parameterSet;
  };

  ParameterSetMap(Int maxId)
  :m_maxId (maxId)
  ,m_activePsId(-1)
  ,m_lastActiveParameterSet(NULL)
  {}

  ~ParameterSetMap()
  {
    for (typename std::map<Int,MapData<T> >::iterator i = m_paramsetMap.begin(); i!= m_paramsetMap.end(); i++)
    {
      delete (*i).second.pNaluData;
      delete (*i).second.parameterSet;
    }
    delete m_lastActiveParameterSet; m_lastActiveParameterSet = NULL;
  }

  T *allocatePS(const Int psId)
  {
    CHECK( psId >= m_maxId, "Invalid PS id" );
    if ( m_paramsetMap.find(psId) == m_paramsetMap.end() )
    {
      m_paramsetMap[psId].bChanged = true;
      m_paramsetMap[psId].pNaluData=0;
      m_paramsetMap[psId].parameterSet = new T;
      setID(m_paramsetMap[psId].parameterSet, psId);
    }
    return m_paramsetMap[psId].parameterSet;
  }

  void storePS(Int psId, T *ps, const std::vector<uint8_t> *pNaluData)
  {
    CHECK( psId >= m_maxId, "Invalid PS id" );
    if ( m_paramsetMap.find(psId) != m_paramsetMap.end() )
    {
      MapData<T> &mapData=m_paramsetMap[psId];

      // work out changed flag
      calculateParameterSetChangedFlag(mapData.bChanged, mapData.pNaluData, pNaluData);

      if( ! mapData.bChanged )
      {
        // just keep the old one
        delete ps;
        return;
      }

      if( m_activePsId == psId )
      {
        std::swap( m_paramsetMap[psId].parameterSet, m_lastActiveParameterSet );
      }
      delete m_paramsetMap[psId].pNaluData;
      delete m_paramsetMap[psId].parameterSet;

      m_paramsetMap[psId].parameterSet = ps;
    }
    else
    {
      m_paramsetMap[psId].parameterSet = ps;
      m_paramsetMap[psId].bChanged = false;
    }
    if (pNaluData != 0)
    {
      m_paramsetMap[psId].pNaluData=new std::vector<uint8_t>;
      *(m_paramsetMap[psId].pNaluData) = *pNaluData;
    }
    else
    {
      m_paramsetMap[psId].pNaluData=0;
    }
  }

  void setChangedFlag(Int psId, bool bChanged=true)
  {
    if ( m_paramsetMap.find(psId) != m_paramsetMap.end() )
    {
      m_paramsetMap[psId].bChanged=bChanged;
    }
  }

  void clearChangedFlag(Int psId)
  {
    if ( m_paramsetMap.find(psId) != m_paramsetMap.end() )
    {
      m_paramsetMap[psId].bChanged=false;
    }
  }

  bool getChangedFlag(Int psId) const
  {
    const typename std::map<Int,MapData<T> >::const_iterator constit=m_paramsetMap.find(psId);
    if ( constit != m_paramsetMap.end() )
    {
      return constit->second.bChanged;
    }
    return false;
  }

  T* getPS(Int psId)
  {
    typename std::map<Int,MapData<T> >::iterator it=m_paramsetMap.find(psId);
    return ( it == m_paramsetMap.end() ) ? NULL : (it)->second.parameterSet;
  }

  const T* getPS(Int psId) const
  {
    typename std::map<Int,MapData<T> >::const_iterator it=m_paramsetMap.find(psId);
    return ( it == m_paramsetMap.end() ) ? NULL : (it)->second.parameterSet;
  }

  T* getFirstPS()
  {
    return (m_paramsetMap.begin() == m_paramsetMap.end() ) ? NULL : m_paramsetMap.begin()->second.parameterSet;
  }

  void setActive(Int psId ) { m_activePsId = psId;}

private:
  std::map<Int,MapData<T> > m_paramsetMap;
  Int                       m_maxId;
  Int                       m_activePsId;
  T*                        m_lastActiveParameterSet;
  static void setID(T* parameterSet, const Int psId);
};


class ParameterSetManager
{
public:
                 ParameterSetManager();
  virtual        ~ParameterSetManager();

#if HEVC_VPS
  //! store sequence parameter set and take ownership of it
  void           storeVPS(VPS *vps, const std::vector<uint8_t> &naluData)      { m_vpsMap.storePS( vps->getVPSId(), vps, &naluData); };
  //! get pointer to existing video parameter set
  VPS*           getVPS(Int vpsId)                                           { return m_vpsMap.getPS(vpsId); };
  bool           getVPSChangedFlag(Int vpsId) const                          { return m_vpsMap.getChangedFlag(vpsId); }
  void           clearVPSChangedFlag(Int vpsId)                              { m_vpsMap.clearChangedFlag(vpsId); }
  VPS*           getFirstVPS()                                               { return m_vpsMap.getFirstPS(); };
#endif

  //! store sequence parameter set and take ownership of it
  void           storeSPS(SPS *sps, const std::vector<uint8_t> &naluData) { m_spsMap.storePS( sps->getSPSId(), sps, &naluData); };
  //! get pointer to existing sequence parameter set
  SPS*           getSPS(Int spsId)                                           { return m_spsMap.getPS(spsId); };
  bool           getSPSChangedFlag(Int spsId) const                          { return m_spsMap.getChangedFlag(spsId); }
  void           clearSPSChangedFlag(Int spsId)                              { m_spsMap.clearChangedFlag(spsId); }
  SPS*           getFirstSPS()                                               { return m_spsMap.getFirstPS(); };

  //! store picture parameter set and take ownership of it
  void           storePPS(PPS *pps, const std::vector<uint8_t> &naluData) { m_ppsMap.storePS( pps->getPPSId(), pps, &naluData); };
  //! get pointer to existing picture parameter set
  PPS*           getPPS(Int ppsId)                                           { return m_ppsMap.getPS(ppsId); };
  bool           getPPSChangedFlag(Int ppsId) const                          { return m_ppsMap.getChangedFlag(ppsId); }
  void           clearPPSChangedFlag(Int ppsId)                              { m_ppsMap.clearChangedFlag(ppsId); }
  PPS*           getFirstPPS()                                               { return m_ppsMap.getFirstPS(); };

  //! activate a SPS from a active parameter sets SEI message
  //! \returns true, if activation is successful
  // bool           activateSPSWithSEI(Int SPSId);

#if HEVC_VPS
  //! activate a PPS and depending on isIDR parameter also SPS and VPS
#else
  //! activate a PPS and depending on isIDR parameter also SPS
#endif
  //! \returns true, if activation is successful
  bool           activatePPS(Int ppsId, bool isIRAP);

#if HEVC_VPS
  const VPS*     getActiveVPS()const                                         { return m_vpsMap.getPS(m_activeVPSId); };
#endif
  const SPS*     getActiveSPS()const                                         { return m_spsMap.getPS(m_activeSPSId); };

protected:
#if HEVC_VPS
  ParameterSetMap<VPS> m_vpsMap;
#endif
  ParameterSetMap<SPS> m_spsMap;
  ParameterSetMap<PPS> m_ppsMap;

#if HEVC_VPS
  Int m_activeVPSId; // -1 for nothing active
#endif
  Int m_activeSPSId; // -1 for nothing active
};

class PreCalcValues
{
public:
  PreCalcValues( const SPS& sps, const PPS& pps, bool _isEncoder )
    : chrFormat           ( sps.getChromaFormatIdc() )
    , multiBlock422       ( chrFormat == CHROMA_422 && !sps.getSpsNext().getUseQTBT() )
#if JEM_TOOLS && !JVET_K0346
    , noMotComp           ( sps.getSpsNext().getDisableMotCompress() || sps.getSpsNext().getUseSubPuMvp() )
#else
    , noMotComp           ( sps.getSpsNext().getDisableMotCompress() )
#endif
    , maxCUWidth          ( sps.getMaxCUWidth() )
    , maxCUHeight         ( sps.getMaxCUHeight() )
    , maxCUWidthMask      ( maxCUWidth  - 1 )
    , maxCUHeightMask     ( maxCUHeight - 1 )
    , maxCUWidthLog2      ( g_aucLog2[ maxCUWidth  ] )
    , maxCUHeightLog2     ( g_aucLog2[ maxCUHeight ] )
    , minCUWidth          ( sps.getMaxCUWidth()  >> sps.getMaxCodingDepth() )
    , minCUHeight         ( sps.getMaxCUHeight() >> sps.getMaxCodingDepth() )
    , minCUWidthLog2      ( g_aucLog2[ minCUWidth  ] )
    , minCUHeightLog2     ( g_aucLog2[ minCUHeight ] )
    , partsInCtuWidth     ( 1 << sps.getMaxCodingDepth() )
    , partsInCtuHeight    ( 1 << sps.getMaxCodingDepth() )
    , partsInCtu          ( 1 << (sps.getMaxCodingDepth() << 1) )
    , widthInCtus         ( (sps.getPicWidthInLumaSamples () + sps.getMaxCUWidth () - 1) / sps.getMaxCUWidth () )
    , heightInCtus        ( (sps.getPicHeightInLumaSamples() + sps.getMaxCUHeight() - 1) / sps.getMaxCUHeight() )
    , sizeInCtus          ( widthInCtus * heightInCtus )
    , lumaWidth           ( sps.getPicWidthInLumaSamples() )
    , lumaHeight          ( sps.getPicHeightInLumaSamples() )
    , fastDeltaQPCuMaxSize( Clip3(sps.getMaxCUHeight() >> (sps.getLog2DiffMaxMinCodingBlockSize()), sps.getMaxCUHeight(), 32u) )
#if INTRA67_3MPM
    , numMPMs             (NUM_MOST_PROBABLE_MODES)
#else
#if JEM_TOOLS
    , numMPMs             ( sps.getSpsNext().getUseIntra65Ang() ? NUM_MOST_PROBABLE_MODES_67 : NUM_MOST_PROBABLE_MODES )
#else
    , numMPMs             ( NUM_MOST_PROBABLE_MODES )
#endif
#endif
    , noRQT               (  sps.getSpsNext().getUseQTBT() )
    , rectCUs             (  sps.getSpsNext().getUseQTBT() )
    , only2Nx2N           (  sps.getSpsNext().getUseQTBT() )
    , noChroma2x2         ( !sps.getSpsNext().getUseQTBT() )
    , isEncoder           ( _isEncoder )
    , ISingleTree         ( !sps.getSpsNext().getUseQTBT() || !sps.getSpsNext().getUseDualITree() )
    , maxBtDepth          { sps.getSpsNext().getMaxBTDepthI(), sps.getSpsNext().getMaxBTDepth(), sps.getSpsNext().getMaxBTDepthIChroma() }
    , minBtSize           { MIN_BT_SIZE, MIN_BT_SIZE_INTER, MIN_BT_SIZE_C }
    , maxBtSize           { sps.getSpsNext().getMaxBTSizeI(), sps.getSpsNext().getMaxBTSize(), sps.getSpsNext().getMaxBTSizeIChroma() }
    , minTtSize           { MIN_TT_SIZE, MIN_TT_SIZE_INTER, MIN_TT_SIZE_C }
    , maxTtSize           { MAX_TT_SIZE, MAX_TT_SIZE_INTER, MAX_TT_SIZE_C }
    , minQtSize           { sps.getSpsNext().getMinQTSize( I_SLICE, CHANNEL_TYPE_LUMA ), sps.getSpsNext().getMinQTSize( B_SLICE, CHANNEL_TYPE_LUMA ), sps.getSpsNext().getMinQTSize( I_SLICE, CHANNEL_TYPE_CHROMA ) }
  {}

  const ChromaFormat chrFormat;
  const bool         multiBlock422;
  const bool         noMotComp;
  const unsigned     maxCUWidth;
  const unsigned     maxCUHeight;
  // to get CTU position, use (x & maxCUWidthMask) rather than (x % maxCUWidth)
  const unsigned     maxCUWidthMask;
  const unsigned     maxCUHeightMask;
  const unsigned     maxCUWidthLog2;
  const unsigned     maxCUHeightLog2;
  const unsigned     minCUWidth;
  const unsigned     minCUHeight;
  const unsigned     minCUWidthLog2;
  const unsigned     minCUHeightLog2;
  const unsigned     partsInCtuWidth;
  const unsigned     partsInCtuHeight;
  const unsigned     partsInCtu;
  const unsigned     widthInCtus;
  const unsigned     heightInCtus;
  const unsigned     sizeInCtus;
  const unsigned     lumaWidth;
  const unsigned     lumaHeight;
  const unsigned     fastDeltaQPCuMaxSize;
  const unsigned     numMPMs;
  const bool         noRQT;
  const bool         rectCUs;
  const bool         only2Nx2N;
  const bool         noChroma2x2;
  const bool         isEncoder;
  const bool         ISingleTree;

private:
  const unsigned     maxBtDepth[3];
  const unsigned     minBtSize [3];
  const unsigned     maxBtSize [3];
  const unsigned     minTtSize [3];
  const unsigned     maxTtSize [3];
  const unsigned     minQtSize [3];

  unsigned getValIdx    ( const Slice &slice, const ChannelType chType ) const;

public:
  unsigned getMaxBtDepth( const Slice &slice, const ChannelType chType ) const;
  unsigned getMinBtSize ( const Slice &slice, const ChannelType chType ) const;
  unsigned getMaxBtSize ( const Slice &slice, const ChannelType chType ) const;
  unsigned getMinTtSize ( const Slice &slice, const ChannelType chType ) const;
  unsigned getMaxTtSize ( const Slice &slice, const ChannelType chType ) const;
  unsigned getMinQtSize ( const Slice &slice, const ChannelType chType ) const;
};

#if ENABLE_TRACING
#if HEVC_VPS
void xTraceVPSHeader();
#endif
void xTraceSPSHeader();
void xTracePPSHeader();
void xTraceSliceHeader();
void xTraceAccessUnitDelimiter();
#endif

#endif // __SLICE__
