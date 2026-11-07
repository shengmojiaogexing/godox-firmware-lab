#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""pdftext.py <pdf> -- extract text from a PDF."""
import sys
import pypdf

r = pypdf.PdfReader(sys.argv[1])
print('pages:', len(r.pages))
for i, p in enumerate(r.pages):
    print(f'\n===== PAGE {i+1} =====')
    try:
        print(p.extract_text())
    except Exception as e:
        print('ERR', e)
