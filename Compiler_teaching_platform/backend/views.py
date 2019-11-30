# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.shortcuts import render
from django.http.response import HttpResponse
from django.http import JsonResponse
from tiger.judge import judge

# Create your views here.
def homePage(request):
    return HttpResponse("<h1>This is home page</h1>")

def test(request):
    return JsonResponse({"status" : 0, "message" : judge("tttttt")})
