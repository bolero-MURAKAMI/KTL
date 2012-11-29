# KTL (KiriKiri Trivial Library) DLL

吉里吉里２のための拡張機能ライブラリ KTL の DLL プロジェクト  



## ドキュメント *(Documents)*

KTLWiki: http://www.boleros.x0.com/doc/ktlwiki/index.php?KTLWiki  



## 謝辞 *(Acknowledgment)*

KTL は下記サードパーティライブラリを利用しています。  
有用なライブラリを公開されている方々に感謝いたします。  

* tp_stub - TVP2 ( T Visual Presenter 2 )  A script authoring tool  
https://sv.kikyou.info/trac/kirikiri/browser/kirikiri2/trunk/kirikiri2/src/plugins/win32  
Copyright (C) 2000-2009 W.Dee <dee@kikyou.info> and contributors  

* ncbind - 吉里吉里プラグイン向けネイティブクラスバインダ  
https://sv.kikyou.info/trac/kirikiri/browser/kirikiri2/trunk/kirikiri2/src/plugins/win32/ncbind  
Author: miahmie  

* Boost C++ Libraries  
http://www.boost.org/  

* Hamigaki C++ Libraries  
http://hamigaki.sourceforge.jp/  
Copyright (C) 2006-2008 Takeshi Mouri  

* Sprig C++ Libraries  
https://github.com/bolero-MURAKAMI/Sprig  
Copyright (C) 2009-2012 Bolero MURAKAMI.  

* zlib  
http://zlib.net/  
Copyright (C) 1995-2012 Jean-loup Gailly and Mark Adler  

* libbzip2  
http://www.bzip.org/  
copyright (C) 1996-2010 Julian R Seward.  All rights reserved.  

* ICU - International Components for Unicode  
http://site.icu-project.org/  
Copyright (C) 2010, International Business Machines Corporation and others. All Rights Reserved.  

* htmlcxx - html and css APIs for C++  
http://htmlcxx.sourceforge.net/
Davi de Castro Reis - davi (a) users sf net  
Robson Braga Araujo - braga (a) users sf net  

* yaml-cpp  
http://code.google.com/p/yaml-cpp/  
Copyright (c) 2008 Jesse Beder.  

* OpenSSL: The Open Source toolkit for SSL/TLS  
http://www.openssl.org/  
Copyright (c) 1998-2011 The OpenSSL Project.  All rights reserved.  

* libpng  
http://www.libpng.org/pub/png/libpng.html  
Copyright (c) 1998-2011 Glenn Randers-Pehrson  
Copyright (c) 1996-1997 Andreas Dilger  
Copyright (c) 1995-1996 Guy Eric Schalnat, Group 42, Inc.  

* NTL: A Library for doing Number Theory  
http://www.shoup.net/ntl/  
Author: Victor Shoup (victor@shoup.net)  

* DirectX SDK  
http://msdn.microsoft.com/ja-jp/directx  



## プラグイン別サードパーティライブラリ *(3rd Perty Libraries)*

各 DLL のビルドには、それぞれ以下のライブラリが必要となります。  

* KTL.Utils  
tp_stub  
ncbind  
Sprig  
Boost  

* KTL.Network  
tp_stub  
ncbind  
Sprig  
Boost  
OpenSSL  

* KTL.LocalFiles  
tp_stub  
ncbind  
Sprig  
Boost  

* KTL.StorageStream  
tp_stub  
ncbind  
Sprig  
Boost  

* KTL.Compression  
tp_stub  
ncbind  
Sprig  
Boost  
zlib  

* KTL.Bzip2  
tp_stub  
ncbind  
Sprig  
Boost  
libbzip2  

* KTL.Checksum  
tp_stub  
ncbind  
Sprig  
Boost  
Hamigaki  

* KTL.OctetBuilder  
tp_stub  
ncbind  
Sprig  
Boost  

* KTL.Encoding  
tp_stub  
ncbind  
Sprig  
Boost  
ICU  

* KTL.PTree  
tp_stub  
ncbind  
Sprig  
Boost  
ICU  
yaml-cpp  
htmlcxx  

* KTL.PTime  
tp_stub  
ncbind  
Sprig  
Boost  

* KTL.VariateRandom  
tp_stub  
ncbind  
Sprig  
Boost  

* KTL.BigInt  
tp_stub  
ncbind  
Sprig  
Boost  
NTL  

* KTL.MathEx  
tp_stub  
ncbind  
Sprig  
Boost  

* KTL.SplashWindow  
tp_stub  
ncbind  
Sprig  
Boost  
zlib  
libpng  

* KTL.Serialization  
tp_stub  
ncbind  
Sprig  
Boost  

* KTL.SpriteCoord  
tp_stub  
ncbind  
Sprig  
Boost  

* KTL.Functor  
tp_stub  
ncbind  
Sprig  
Boost  

* KTL.Sprites
tp_stub  
ncbind  
Sprig  
Boost  
DirectX SDK  

* KTL.OSVersion
tp_stub  
ncbind  
Sprig  
Boost  



## 作者 *(Author)*

Bolero MURAKAMI  

Website: http://www.boleros.x0.com/  
Twitter: https://twitter.com/bolero_murakami  
Facebook: http://www.facebook.com/genya.murakami  
Blog: http://d.hatena.ne.jp/boleros/  
Github: https://github.com/bolero-MURAKAMI  
SlideShare: http://www.slideshare.net/GenyaMurakami  
Mail: contact-lib@boleros.x0.com  



## 著作権等 *(Copyrights)*

このライブラリは Boost Software License の元で公開されています。  

Copyright (C) 2011-2012 Bolero MURAKAMI.  
Distributed under the Boost Software License, Version 1.0.  
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)  
